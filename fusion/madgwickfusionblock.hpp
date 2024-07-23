/**############################################################################
#
# Description: Implementation of the revised Madgwick Fusion Algorithm
#
#
# Copyright (C) 2021 by Hristina Radak
#
# Email: hristinaradak95@gmail.com
#
##############################################################################################################################################################
# This code represents the implementation of the revised Madgwick fusion
# algorithm (from the PhD Thesis Chapter 7, Fig. 7.1) for orientation estimation using 3D measurements
# from gyroscope, accelerometer and magnetometer measurements
###############################################################################*/

#ifndef __MADGWICKFUSIONBLOCK_H
#define __MADGWICKFUSIONBLOCK_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include <armadillo>
#include <cfloat>
#include <iostream>
#include <cmath>

#include "../tools/quaternion/quaternion.hpp"

/**############################################################################
# NAMES
#############################################################################*/

using namespace std;
using namespace arma;


/**############################################################################
# DEFINES
#############################################################################*/


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/


/******************************************************************************
 * A Exception class that refers to the @ref MadgwickFusionBlock class.
 *****************************************************************************/
class MadgwickFusionBlockExcept {
public:
  /** a list of known possible errors with class MadgwickFusionBlock *********/
  //enum {};

  /** the number of error ****************************************************/
  int num;

  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   ***************************************************************************/
  MadgwickFusionBlockExcept(int _num);
};


/******************************************************************************
 *****************************************************************************/
class MadgwickFusionBlock{// : public FusionBlock {
	/****************************************************************************
	***************************************************************************/
	const double MIN_MAG_FIELD = 0.0;
	const double MAX_MAG_FIELD = 80.0;

	const double DEFAULT_GAIN = 10.0;
	const double DEFAULT_PERIOD = 3.0;

	/** 3D column vector type **************************************************/
	typedef arma::Col<double>::fixed<3> colvec;


  /****************************************************************************
   ***************************************************************************/
	double ramped_beta_;
  /****************************************************************************
   ***************************************************************************/
	double beta_;
  /****************************************************************************
   ***************************************************************************/
	double samplingTime_;

  /****************************************************************************
   *
   ***************************************************************************/
	Quaternion q_ = Quaternion(0.0, 1.0, 0.0, 0.0);

	Quaternion gyro_, acc_, mag_, q_dot_;

	colvec gravity_, west_, e_acc_, e_mag_;
	colvec err_;




  /****************************************************************************
   ***************************************************************************/

public:
  /****************************************************************************
   ***************************************************************************/

	MadgwickFusionBlock(){
		beta_ = 0.1;
		ramped_beta_ = DEFAULT_GAIN;
		q_ = Quaternion(1.0, 0.0, 0.0, 0.0);
		samplingTime_ = 0;
	}
	MadgwickFusionBlock(double beta)
	{
		beta_ = beta;
		ramped_beta_ = DEFAULT_GAIN;
		q_ = Quaternion(0,1,0,0);
		samplingTime_ = 0;
	}

  /****************************************************************************
   ***************************************************************************/
  ~MadgwickFusionBlock()
  {
  }
  /******************************************************************************
   * update the value of ramped_beta following Eq. 7.6
   *****************************************************************************/
  void ramp_beta_down()
  {
	//skip initialization if gain is zero
	if(beta_ == 0) ramped_beta_ = 0;

	if(ramped_beta_ > beta_){
		ramped_beta_ -= (DEFAULT_GAIN - beta_) * samplingTime_/ DEFAULT_PERIOD;
	}
	else{
		ramped_beta_ = beta_;
	}
  }
  /******************************************************************************
   * convert measurement results to radians per second
   *****************************************************************************/
  double degree_to_radian(const double degree)
  {
	return degree * ( M_PI / 180.0);
  }
  /******************************************************************************
   * check whether the mag measurement is valid or not
   *****************************************************************************/
  bool is_mag_valid(const Quaternion & q)
  {
	double sum_square = q.v1() * q.v1() + q.v2() * q.v2() + q.v3() * q.v3();
	if((sum_square < MIN_MAG_FIELD * MIN_MAG_FIELD) || (sum_square > MAX_MAG_FIELD * MAX_MAG_FIELD))
	{
		return false;
	}
	return true;
  }
	/******************************************************************************
	* check whether the acc measurement is valid or not
	*****************************************************************************/
  bool is_acc_valid(const Quaternion & q)
   {

   	if((q.v1() == 0.0) && (q.v2() == 0.0) && (q.v3() == 0.0))
   	{
   		return false;
   	}
   	return true;
   }

  /******************************************************************************
   * Calculate the accelerometer error component using Quaternion
   * e_acc = a/||a||x[(2q_xq_z - 2q_wq_y)(2q_yq_z - 2 q_wq_x)(2q_wq_w - 1 + 2q_zq_z)]
   *****************************************************************************/
  void cal_acc_error()
  {
  	gravity_(0) =	q_.v1() * q_.v3() - q_.s()  * q_.v2();
  	gravity_(1) =	q_.v2() * q_.v3() + q_.s()  * q_.v1();
  	gravity_(2) =	q_.s()  * q_.s()  + q_.v3() * q_.v3() - 0.5;
  	gravity_ *= 2;

  	e_acc_ = arma::normalise(acc_.v(), 2); //check the normalize function

    e_acc_ = arma::cross(e_acc_, gravity_);

  }
  /******************************************************************************
   * Calculate the magnetometer error component using Quaternion
   * e_mag = (axm)/||axm||x [(2q_xq_y + 2q_wq_z)(2q_wq_w - 1 + 2q_yq_y)(2q_yq_z - 2q_wq_x)]
   *****************************************************************************/
  void cal_mag_error()
  {
  	west_(0) = q_.v1() * q_.v2() + q_.s()  * q_.v3();
  	west_(1) = q_.s()  * q_.s()  + q_.v2() * q_.v2() - 0.5;
  	west_(2) = q_.v2() * q_.v3() - q_.s()  * q_.v1();
  	west_ *= 2;
  	e_mag_ = arma::cross(acc_.v(), mag_.v());

  	e_mag_ = arma::normalise(e_mag_, 2);

  	e_mag_ = arma::cross(e_mag_, west_);

  }

  const Quaternion & run(const Quaternion & gyro,
		  	  	  	  	 const Quaternion & acc,
						 const Quaternion & mag,
						 const double &samplingTime,
						 const Quaternion &q)
  {
    samplingTime_ = samplingTime;

  	gyro_ = gyro; //* degree_to_radian(1.0);
  	acc_  = acc;
  	mag_  = mag;
  	q_ = q;

  	//Check if the accelerometer measurement is valid
	if(is_acc_valid(acc))
	{
		cal_acc_error();
	}

  	//Check if the accelerometer measurement is valid
  	if(is_mag_valid(mag))
  	{
  		//Calculate the magnetometer error component e_mag (Eq. 7.4)
		cal_mag_error();
  	}

  	//Calculate the error component err
  	err_.fill(0.0);

  	if(is_acc_valid(acc) && is_mag_valid(mag))
  	{
  		err_ = e_acc_ + e_mag_;
  	}
  	else
  		if (! is_mag_valid(mag))
  		{
  			err_ = e_acc_;
  		}

  	/******************************************************************************
  	 * Calculate the quaternion derivative (Eq. 7.2)
  	 * q_dot = 1/2 * q_ * (gyro + beta_ * err)
  	 * q_ is now q_dot_
  	 *****************************************************************************/
  	ramp_beta_down();

  	err_ *= ramped_beta_;
  	err_ *= beta_;
  	gyro_ += err_;
  	q_dot_ = q_;
  	q_dot_ *= gyro_; //calculate rampedGain, see halfGyroscope in original alg. (tomorrow)
  	q_dot_ *= 0.5;

  	//Integrate (Eq. 7.1)
  	q_dot_ *= samplingTime;
  	q_ += q_dot_;

  	q_.to_normalized();

  	return q_;
  }

};


/**############################################################################
# INLINE MEMBER DEFINITIONS
#############################################################################*/



/**############################################################################
# TEMPLATE MEMBER DEFINITIONS
#############################################################################*/


/**############################################################################
# NON MEMBER DECLARATIONS
#############################################################################*/


/**############################################################################
# END OF FILE
#############################################################################*/

#endif /* __MADGWICKFUSIONBLOCK_H ********************************************/
