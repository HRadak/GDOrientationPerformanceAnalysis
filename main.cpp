/**############################################################################
#
# Description: Orientation estimation framework
#
#
# Copyright (C) 2024 by Hristina Radak
#
# Email: hristinaradak95@gmail.com
#
###############################################################################
# This code represents the implementation of the orientation estimation
# framework. We implemented the original Madgwick, the Wilson, and the novel
# QGD-OE algorithm for orientation estimation using 3D measurements
# from gyroscope, accelerometer and magnetometer
#############################################################################*/

/**############################################################################
 # INCLUDES
 #############################################################################*/

#include <armadillo>
#include <filesystem>
#include <iostream>
#include <string>
#include <valarray>
#include <deque>
#include <list>
#include <random>

#include "./tools/quaternion/quaternion.hpp"
#include "./fusion/wilsonfusionblock.hpp"
#include "./fusion/madgwick-original.hpp"
#include "./fusion/qgdfusionblock.hpp"
#include "./fusion/madgwickfusionblock.hpp"

#include "./tools/common/csv_writer.hpp"
#include "./io/convert.h"
#include "./io/ioconfigfile.h"
#include "./io/iodirectory.h"
#include "./io/iodatafile.h"
#include "./io/iooption.h"
#include "./sense.h"

/**############################################################################
 # OBJECTS
 #############################################################################*/

IoDataFile ioData_;
IoConfigFile ioConf;
IoDirectory ioDir;

std::uniform_real_distribution<double> unif(0,1);
std::default_random_engine re;

/**############################################################################
 # VARIABLES
 #############################################################################*/

unsigned numSamples_;
valarray<double> tmpSamples;

Mat<double> gyroData_, quatData_, gyroData_smooth, accData_smooth, magData_smooth;
Mat<double> accData_;
Mat<double> magData_;
Col<double> timeData_;

deque<Col<double>::fixed<4>> buffer;
deque<double> bufferScalar;

Quaternion qM1_, qM2_, qW_, qQ_, q_conj_, q_help_;
Quaternion gyro_, acc_, mag_, gyro_smooth, acc_smooth, mag_smooth;
Quaternion qTrue_, qTrue_conj, qQGD_conj, qWilson_conj, q_mdw1_conj, q_mdw2_conj;
Quaternion acc_mdw, mag_mdw;
Quaternion magRef_;
Quaternion q_relative_;

arma::Col<double>::fixed<3> anglesQ_, anglesW_, anglesT_, anglesM1_, anglesM2_, u;
Col<double> beta_;

double samplingTime_;

/****************************************************************************
***************************************************************************/

constexpr double pi() {
	return std::atan(1) * 4;
}

/****************************************************************************
 * create a list of beta values on a base 10 logarithmic scale
***************************************************************************/
vector<double> genBeta(double first, double last)
{
	vector<double> betas;
	double beta;
	for(double base = first; base<=last;base*=10){
		for(unsigned int i=1;i<10;i++){
			beta = base*i;
			betas.push_back(beta);
		}
	}
	return betas;
}

/****************************************************************************
 * moving average filter
***************************************************************************/
void movingAvg(Mat<double> &data, unsigned int filterSize){
	Col<double>::fixed<4> avg;
	for(unsigned int k=0; k<numSamples_; k++){
	    if(k == 0){
	    	buffer.clear();
	    	avg = {0,0,0,0};
	    }
		avg += data.col(k)/filterSize;
		buffer.push_front(data.col(k));
		if(buffer.size() == filterSize){
			avg -= buffer.back()/filterSize;
			data.submat(0,k,3,k) = avg;
			buffer.pop_back();
		}
	}
}

/****************************************************************************
 * convert frame of a quaternion q representing acc/mag measurement to adapt
 * the original Madgwick result representation to Wilson and QGD-OE
 * representation
***************************************************************************/
void convertFrame(Quaternion &q){
	  q_conj_ = Quaternion(0,1,0,0);
	  q_help_ = q;
	  q_help_ *= q_conj_;
	  q_conj_.to_conj();
	  q_conj_*=q_help_;
	  q = q_conj_;
}

void deleteDirectoryContents(const std::string& dir_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
        std::filesystem::remove_all(entry.path());
}

/****************************************************************************
 * calculate equivalemt magnetometer measurement quaternion v_m = v_a × v_m = - v_m × v_a
 ***************************************************************************/
void MagEquivalent()
{
	mag_.cross(acc_);
	mag_.to_conj();
	mag_.to_normalized();
}

/** get random quaternion that represents rotation around a random axis by u
 * by a random angle theta, when |theta| < 10 degrees **/
Quaternion getRandomQuaternion()
{
   for(unsigned int i=0;i<3;i++){
		   u(i) = unif(re);
   }
   double theta = unif(re)*10; // random angle between -10 and 10 degrees
   theta = theta*pi()/180; // convert angle from degrees to radians
   Quaternion random_quat = Quaternion(u, theta); // construct quaternion from axis and angle
   return random_quat;
}

int runFusions(const string &_confFileName) {
	string Mode, DataSource, GyroData, AccData, MagData, QuatData,QuatDataResult,EulerDataResult;
//	const string Mode = argv[1];
//	const string DataSource = argv[2];
//	const string i = argv[3];

	/** load the configuration file ******************************************/
	ioConf.loadFile(_confFileName, false);

	/** get config data from global section **********************************/
	ioConf.getValue(Mode, "Mode");
	ioConf.getValue(DataSource, "DataSource");
	ioConf.getValue(GyroData, "GyroData");
	ioConf.getValue(AccData, "AccData");
	ioConf.getValue(MagData, "MagData");
	ioConf.getValue(QuatData, "QuatData");
	ioConf.getValue(QuatDataResult, "QuatDataResult");
	ioConf.getValue(EulerDataResult, "EulerDataResult");

	/*************************************************************************
	 * define  folders
	 ***********************************************************************/
	const string folderIn;// = "ExampleData/";
	const string folderOut = "Results/";

	magRef_ = Quaternion(0,0.391801903,0,0.920049601); // input user inclination for MDW1 algorithm
	beta_ = genBeta(0.01,1000);

	/*************************************************************************
	 * create results folders
	 ***********************************************************************/
	ioDir.create(folderOut + QuatDataResult);
	ioDir.create(folderOut + EulerDataResult);

	/*************************************************************************
	 * read gyroscope samples as quaternion:
	 * gyroData_ = {0, gyro_x, gyro_y, gyro_z}
	 ***********************************************************************/
	ioData_.setFileName(folderIn + GyroData);

	ioData_.getValue(numSamples_, "numSamples", "GLOBAL_DATA");
	gyroData_.zeros(4, numSamples_);

	ioData_.getValues(tmpSamples, "gyro_x", "GYRO_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		gyroData_(1, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "gyro_y", "GYRO_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		gyroData_(2, i) = tmpSamples[i];//- for mdw dataset
	}
	ioData_.getValues(tmpSamples, "gyro_z", "GYRO_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		gyroData_(3, i) = tmpSamples[i];//- for mdw dataset
	}

	/****************************************************************************
	 * read accelerometer samples as quaternion:
	 * accData_ = {0, acc_x, acc_y, acc_z}
	 ***************************************************************************/
	ioData_.setFileName(folderIn + AccData);
	accData_.zeros(4, numSamples_);

	ioData_.getValues(tmpSamples, "acc_x", "ACC_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		accData_(1, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "acc_y", "ACC_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		accData_(2, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "acc_z", "ACC_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		accData_(3, i) = tmpSamples[i];
	}

	/****************************************************************************
	 * read magnetometer samples as quaternion:
	 * magData_ = {0, mag_x, mag_y, mag_z}
	 ***************************************************************************/
	ioData_.setFileName(folderIn + MagData);

	magData_.zeros(4, numSamples_);

	ioData_.getValues(tmpSamples, "mag_x", "MAG_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		magData_(1, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "mag_y", "MAG_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		magData_(2, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "mag_z", "MAG_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		magData_(3, i) = tmpSamples[i];
	}

//	/****************************************************************************
//	 * read sampling time values
//	 ***************************************************************************/
//	ioData_.setFileName("../ExampleData/SensorData/sampling_time_X1.dat");
//	timeData_.zeros(numSamples_);
//
//	ioData_.getValues(tmpSamples, "time", "TIME_DATA", numSamples_);
//	for (unsigned i = 0; i < numSamples_; i++) {
//		timeData_(i) = tmpSamples[i];
//	}

	/****************************************************************************
	 * read true quaternion:
	 * quat = {qw,qx,qy,qz}
	 ***************************************************************************/
	ioData_.setFileName(folderIn + QuatData);

	quatData_.zeros(4, numSamples_);

	ioData_.getValues(tmpSamples, "quat_w", "QUAT_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		quatData_(0, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "quat_x", "QUAT_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		quatData_(1, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "quat_y", "QUAT_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		quatData_(2, i) = tmpSamples[i];
	}
	ioData_.getValues(tmpSamples, "quat_z", "QUAT_DATA", numSamples_);
	for (unsigned i = 0; i < numSamples_; i++) {
		quatData_(3, i) = tmpSamples[i];
	}

//	deleteDirectoryContents(folderOut + "euler/");
//	deleteDirectoryContents(folderOut + "quat/");

//	gyroData_smooth = gyroData_;
//	accData_smooth = accData_;
//	magData_smooth = magData_;

//	movingAvg(gyroData_smooth, 10);
//	movingAvg(accData_smooth, 30);
//	movingAvg(magData_smooth, 50);

	for (unsigned int j = 0; j < beta_.size(); j++){

		WilsonFusionBlock wilson_(beta_[j]); // Wilson
		Madgwick1FusionBlock mdw1_(beta_[j], magRef_); // Madgwick original
		QuaternionGradientDescentBlock qgd_(beta_[j]); // QGD

//		Quaternion qOldM1_ = Quaternion(  0.264, -0.061, 0.106, -0.957 );
//		Quaternion qOldW_ = Quaternion(  0.264, -0.061, 0.106, -0.957 );
//		Quaternion qOldQ_ = Quaternion(  0.264, -0.061, 0.106, -0.957 );

//		Initialize fusions with quaternion close to true
		qTrue_ = quatData_.col(1);
		q_relative_ = getRandomQuaternion();
		q_relative_ *= qTrue_;
		q_relative_.to_normalized();
		Quaternion qOldM1_ = q_relative_;
		Quaternion qOldW_ = q_relative_;
		Quaternion qOldQ_ = q_relative_;

		buffer.clear();

		/**Start loop to execute the fusion algorithms**/
		for (unsigned i = 1; i < numSamples_; i++) {
			qTrue_ = quatData_.col(i);
			/** get samples from sensors ******************************************/
			gyro_ = gyroData_.col(i);
			gyro_ *= pi() / 180; // convert gyro readings from deg/s to rad/s
//			acc_ = accData_.col(i);

			//*** get true acc measurement from true quaternion ***//
			acc_ = Quaternion(0,0,0,-1);
			qTrue_conj = qTrue_;
			qTrue_conj.to_conj();
			acc_ *= qTrue_;
			qTrue_conj *= acc_;
			acc_ = qTrue_conj;

			mag_ = magData_.col(i);

			acc_mdw = acc_;
			mag_mdw = mag_;

			//*** Moving average filter ***//
//			gyro_smooth = gyroData_smooth.col(i);
//			gyro_smooth *= pi() / 180; // convert gyro readings from deg/s to rad/s

//			acc_smooth = accData_smooth.col(i);
//			mag_smooth = magData_smooth.col(i);

			//*** Computing equivalent magnetometer vector according to Wilson ***//
			MagEquivalent();

			//***  Convert to Madgwick dataset representation (when using Madgwick dataset)***//
			if(DataSource == "MadgwickData"){

				convertFrame(acc_mdw);
				convertFrame(mag_mdw);
			}

			/** run with Madgwick original fusion algorithm *****************************/
			qM1_ = mdw1_.run(gyro_, acc_mdw, mag_mdw, 0.01, qOldM1_);
			qOldM1_ = qM1_;

			/**run with Wilson fusion algorithm************************************/
			qW_ = wilson_.run(gyro_, acc_, mag_, 0.01, qOldW_);
			qOldW_ = qW_;

			/** run with QGD fusion algorithm *****************************/
			qQ_ = qgd_.run(gyro_, acc_, mag_, 0.01, qOldQ_);
			qOldQ_ = qQ_;


			//*** create file index ***//
			string beta_str = "/";
			if(j<10) beta_str += "000";
			else if(j<100) beta_str += "00";
			else if(j<1000) beta_str += "0";
			beta_str += std::to_string(j) + ".csv";

			//*** write quaternion results to file ***//
			string filename = folderOut + QuatDataResult + beta_str;
			write_csv_file(filename,
					std::to_string(qTrue_.s()).c_str(),
					std::to_string(qTrue_.v1()).c_str(),
					std::to_string(qTrue_.v2()).c_str(),
					std::to_string(qTrue_.v3()).c_str(),
					std::to_string(qM1_.s()).c_str(),
					std::to_string(qM1_.v1()).c_str(),
					std::to_string(qM1_.v2()).c_str(),
					std::to_string(qM1_.v3()).c_str(),
					std::to_string(qW_.s()).c_str(),
					std::to_string(qW_.v1()).c_str(),
					std::to_string(qW_.v2()).c_str(),
					std::to_string(qW_.v3()).c_str(),
					std::to_string(qQ_.s()).c_str(),
					std::to_string(qQ_.v1()).c_str(),
					std::to_string(qQ_.v2()).c_str(),
					std::to_string(qQ_.v3()).c_str(),
					std::to_string(beta_[j]).c_str(),
					NULL);

			//*** convert quaternions to Euler angles ***//
			qTrue_conj = qTrue_;
			qTrue_conj.to_conj();
			qTrue_conj.to_EulerAngles(anglesT_);
			anglesT_ *=180/pi();

			q_mdw1_conj = qM1_;
			q_mdw1_conj.to_conj();
			q_mdw1_conj.to_EulerAngles(anglesM1_);
			anglesM1_ *=180/pi();

			qWilson_conj = qW_;
			qWilson_conj.to_conj();
			qWilson_conj.to_EulerAngles(anglesW_);
			anglesW_ *=180/pi();

			qQGD_conj = qQ_;
			qQGD_conj.to_conj();
			qQGD_conj.to_EulerAngles(anglesQ_);
			anglesQ_ *=180/pi();

			//*** write Euler angle results to file ***//
			string filename1 = folderOut + EulerDataResult + beta_str;
			write_csv_file(filename1,
				std::to_string(anglesT_[0]).c_str(),// -
				std::to_string(anglesT_[1]).c_str(),// -
				std::to_string(anglesT_[2]).c_str(),
				std::to_string(anglesM1_[0]).c_str(),
				std::to_string(anglesM1_[1]).c_str(),
				std::to_string(anglesM1_[2]).c_str(),
				std::to_string(anglesW_[0]).c_str(),
				std::to_string(anglesW_[1]).c_str(),
				std::to_string(anglesW_[2]).c_str(),
				std::to_string(anglesQ_[0]).c_str(),
				std::to_string(anglesQ_[1]).c_str(),
				std::to_string(anglesQ_[2]).c_str(),
				std::to_string(beta_[j]).c_str(),
				NULL);

//			string filename5 = "./Results/2023_02_synt/dynamic/imu_data_raw.csv";
//			write_csv_file(filename5,
//				std::to_string(gyro_.v1()).c_str(),
//				std::to_string(gyro_.v2()).c_str(),
//				std::to_string(gyro_.v3()).c_str(),
//				std::to_string(acc_.v1()).c_str(),
//				std::to_string(acc_.v2()).c_str(),
//				std::to_string(acc_.v3()).c_str(),
//				std::to_string(mag_.v1()).c_str(),
//				std::to_string(mag_.v2()).c_str(),
//				std::to_string(mag_.v3()).c_str(),
//				NULL);

		}
	}
	std::cout << "Finished in mode " << Mode << " on data " << DataSource << std::endl;
    return 0;
}

int main(int argc, char *argv[]) {

	SenseOptions senseOptions(argc, argv);
	/** run a simulation for any configuration file found ********************/
	for (size_t i = 0; i < senseOptions.getNumConfFiles(); i++) {

		/** repetitions with the same config file ******************************/
		for (size_t j = 0; j < senseOptions.getNumRepetitions(); j++) {

			/** create the controller for the current run ************************/
			runFusions(senseOptions.getConfFileName(i));

		} /** for (j = 0; j < senseOptions.getNumRepetitions(); j++) ***********/

	} /** for (i = 0; i < senseOptions.getNumConfFiles(); i++) ***************/
}
