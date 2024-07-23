/**############################################################################
#
# Description: Implementation of the Madgwick Fusion Algorithm
#
#
# Copyright (C) 2021 by Hristina Radak
#
# Email: hristinaradak95@gmail.com
#
##############################################################################################################################################################
# This code represents the implementation of the original Madgwick fusion
# algorithm (by Madgwick et. al.2011) for orientation estimation using 3D measurements
# from gyroscope, accelerometer and magnetometer measurements
###############################################################################*/

#ifndef __MADGWICK1FUSIONBLOCK_H
#define __MADGWICK1FUSIONBLOCK_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include <armadillo>
#include <iostream>

#include "../tools/quaternion/quaternion.hpp"

/**############################################################################
# NAMES
#############################################################################*/


/**############################################################################
# DEFINES
#############################################################################*/


/**############################################################################
# CLASS DECLARATIONS
#############################################################################*/


/******************************************************************************
 * A Exception class that refers to the @ref Madgwick1FusionBlock class.
 *****************************************************************************/
class Madgwick1FusionBlockExpect {
public:
  /** a list of known possible errors with class WilsonFusionBlock *********/
  //enum {};

  /** the number of error ****************************************************/
  int num;

  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   ***************************************************************************/
  Madgwick1FusionBlockExpect(int _num);
};


/******************************************************************************
 *****************************************************************************/
class Madgwick1FusionBlock {//: public FusionBlock {

  /** 3D column vector type **************************************************/
	typedef arma::Col<double>::fixed<3> colvec;

  /** 4D column vector type **************************************************/
	typedef arma::Col<double>::fixed<4> colvec4;

	/** 4x3 matrix type ********************************************************/
	typedef arma::Mat<double>::fixed<4,3> mat4x3;

  /****************************************************************************
   ***************************************************************************/
	double beta_;

  /****************************************************************************
   ***************************************************************************/
	double samplingTime_;

  /****************************************************************************
   ***************************************************************************/
	Quaternion gyro_;
	Quaternion acc_;
	Quaternion mag_;

	Quaternion q_ = {0,1,0,0 };
	Quaternion q_dot_;
	Quaternion gradFa_, gradFm_;

	mat4x3 Ja_;
	mat4x3 Jm_;
	colvec fa_;
	colvec fm_;
	colvec4 vector_;

	Quaternion magRef_;

	/****************************************************************************
    * max magnitude of the Earth's magnetic field in units of Gauss
    ***************************************************************************/
	const double M_MIN_ = -80;//-0.65;

  /****************************************************************************
   *min magnitude of the Earth's magnetic field in units of Gauss
   ***************************************************************************/
	const double M_MAX_ = 80;//0.65

public:
  /****************************************************************************
   * beta = 0.01 for fast movement or beta = 2.0 for slow movement
   ***************************************************************************/
	Madgwick1FusionBlock(const double beta = 2.0, const Quaternion magRef = {0,1,0,0})
  {
  	setBeta(beta);
  	setMagRef(magRef);
  }

  /****************************************************************************
   ***************************************************************************/
  ~Madgwick1FusionBlock()
  {
  	setBeta(0);
  }

  /****************************************************************************
   ***************************************************************************/
  void setBeta(const double beta)
  {
  	beta_ = beta;
  }

  /****************************************************************************
   ***************************************************************************/
  double getBeta()
  {
  	return beta_;
  }

  void setMagRef(const Quaternion magRef)
  {
  	magRef_ = magRef;
  }

  /****************************************************************************
   * integrate q_dot_ to obtain q_
   ***************************************************************************/
  void integrate(const double samplingTime)
  {
  	if ( samplingTime > 0 ) { samplingTime_ = samplingTime;	}
  	/** calculate q_ = q_dot_ * samplingTime_ ********************************/
  	q_dot_ *= samplingTime_;
  	q_ += q_dot_;
  }

  /** calculate Jacobian matrices ********************************************/
  void Jacobian()
  {
		Ja_(0,0) = q_.v2();
		Ja_(0,1) = -q_.v1();
		Ja_(0,2) = -q_.s();
		Ja_(1,0) = -q_.v3();
		Ja_(1,1) = -q_.s();
		Ja_(1,2) = q_.v1();
		Ja_(2,0) = q_.s();
		Ja_(2,1) = -q_.v3();
		Ja_(2,2) = q_.v2();
		Ja_(3,0) = -q_.v1();
		Ja_(3,1) = -q_.v2();
		Ja_(3,2) = -q_.v3();
		Ja_ *= 2;

		Jm_(0,0) = magRef_.v1() * q_.s() - magRef_.v3()*q_.v2();
		Jm_(0,1) = -magRef_.v1() * q_.v3() + magRef_.v3()*q_.v1();
		Jm_(0,2) = magRef_.v1() * q_.v2() + magRef_.v3()*q_.s();
		Jm_(1,0) = magRef_.v1() * q_.v1() + magRef_.v3()*q_.v3();
		Jm_(1,1) = Jm_(0,2);
		Jm_(1,2) = -Jm_(0,1);
		Jm_(2,0) = -Jm_(0,2);
		Jm_(2,1) = Jm_(1,0);
		Jm_(2,2) = Jm_(0,0);
		Jm_(3,0) = Jm_(0,1);
		Jm_(3,1) = -Jm_(0,0);
		Jm_(3,2) = Jm_(1,0);
		Jm_ *= 2;
  }

  /** calculate difference equations ****************************************/
  void DifferenceEquations()
  {
	double help_;
  	fa_(0) = q_.s()*q_.v2() - q_.v1()*q_.v3();
  	fa_(1) =-q_.s()*q_.v1() - q_.v2()*q_.v3();
  	fa_ *= 2;
  	fa_(2) = -q_.s()*q_.s() + q_.v1()*q_.v1() + q_.v2()*q_.v2() - q_.v3()*q_.v3();
  	fa_ -= acc_.v();

  	fm_(0) = q_.s()*q_.s() + q_.v1()*q_.v1() - q_.v2()*q_.v2() - q_.v3()*q_.v3();
  	fm_(0) *= magRef_.v1();
  	help_ = -q_.s()*q_.v2() - q_.v1()*q_.v3();
  	help_ *= magRef_.v3();
  	fm_(0) += help_;

  	fm_(1) = -q_.s()*q_.v3() + q_.v1()*q_.v2();
  	fm_(1) *= magRef_.v1();
  	help_ = q_.s()*q_.v1() + q_.v2()*q_.v3();
  	help_ *= magRef_.v3();
  	fm_(1) += help_;

  	fm_(2) = q_.s()*q_.v2() + q_.v1()*q_.v3();
  	fm_(2) *= magRef_.v1();
  	help_ = q_.s()*q_.s() - q_.v1()*q_.v1() - q_.v2()*q_.v2() + q_.v3()*q_.v3();
  	help_ *= magRef_.v3();
  	fm_(2) += help_;
  	fm_ -= mag_.v();

  }


  /****************************************************************************
   *Reject magnetometer measurements that deviate from the expected intensity of the Earth's
   *magnetic field
   ***************************************************************************/

  void MagDistRejection()
  {
  	if ( mag_.abs() < M_MIN_ || mag_.abs() > M_MAX_ ) { mag_.to_zero(); }
  }

  /****************************************************************************
   * Implementation of the Wilson fusion algorithm
   ***************************************************************************/
	const Quaternion run(const Quaternion &gyro,
			const Quaternion &acc,
			const Quaternion &mag,
			const double &samplingTime,
			const Quaternion &q)

	{
		/** get samples from sensors *****************************************/
		gyro_ = gyro;
		acc_ = acc;
		mag_ = mag;
		q_ = q;
		samplingTime_ = samplingTime;
		acc_.to_normalized();
		mag_.to_normalized();

		/** reject magnetometer distortion ***********************************/
//		MagDistRejection();

		/** upper branch **/
		/** calculate Jacobian and difference equations **********************/
		Jacobian();
		DifferenceEquations();

		/** calculate and normalize the gradient******************************/
		gradFa_ = Quaternion(Ja_ * fa_ + Jm_ * fm_); // uncomment + Jm_ * fm to include mag. data for GD
		gradFa_.to_normalized();
//		std::cout << "norm" << gradFa_.norm() << std::endl;
		gradFa_ *= beta_;

		/** lower branch: q_ becomes q_dot_ from here ************************/
		q_dot_ = q_;
		q_dot_ *= gyro_;
		q_dot_ *= 0.5;
		q_dot_ -= gradFa_;
		integrate(samplingTime_);

//		q_ -= gradFa_;
		/** normalize quaternion **********************************************/
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

#endif /* __WILSONFUSIONBLOCK_H **********************************************/
