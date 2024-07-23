/**############################################################################
#
# Description: Implementation of the Quaternion Gradient Descent Orientation Estimation Algorithm
#
#
# Copyright (C) 2021 by Hristina Radak
#
# Email: hristinaradak95@gmail.com
#
###############################################################################
# This code represents the implementation of the quaternion gradient descent
# algorithm for orientation estimation using 3D measurements
# from gyroscope, accelerometer and magnetometer
#############################################################################*/


#ifndef __QUATERNIONGRADIENTDESCENTBLOCK_H
#define __QUATERNIONGRADIENTDESCENTBLOCK_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include <armadillo>
#include <iostream>

#include "../tools/quaternion/quaternion.hpp"

/******************************************************************************
 * A Exception class that refers to the @ref QGDFusionBlock class.
 *****************************************************************************/
class QuaternionGradientDescentBlockExcept {
public:
  /** a list of known possible errors with class QGDFusionBlock *********/
  //enum {};

  /** the number of error ****************************************************/
  int num;

  /****************************************************************************
   * This construction function is used to set the number of exception.
   * @param _num Number of error that has been found.
   ***************************************************************************/
  QuaternionGradientDescentBlockExcept(int _num);
};


/******************************************************************************
 *****************************************************************************/
class QuaternionGradientDescentBlock {
private:
	  /** 3D column vector type **************************************************/
		typedef arma::Col<double>::fixed<3> colvec;

	  /** 4D column vector type **************************************************/
		typedef arma::Col<double>::fixed<4> colvec4;

		/** 4x3 matrix type ********************************************************/
		typedef arma::Mat<double>::fixed<4,3> mat4x3;

		/** 4x4 matrix type ********************************************************/
		typedef arma::Mat<double>::fixed<4,4> mat4x4;
	  /****************************************************************************
	   ***************************************************************************/
		double beta_, beta_a_, beta_m_;

	  /****************************************************************************
	   ***************************************************************************/
		double samplingTime_, norm_;

	  /****************************************************************************
	   ***************************************************************************/
		Quaternion gyro_;
		Quaternion acc_, acc_old_;
		Quaternion mag_;
		colvec fa_, fm_, faOld_, fmOld_;

		Quaternion q_, q_conj_, q_help_ = Quaternion(1,0,0,0);
		Quaternion q_dot_;
		Quaternion gradF_, gradFa_, gradFm_;

		mat4x4 Ma_;
		mat4x4 Mm_;
		colvec4 vec_acc, vec_mag;

		double q_norm_, norm_fa_, norm_fm_;

		/****************************************************************************
	    * max magnitude of the Earth's magnetic field in units of Gauss
	    ***************************************************************************/
		const double M_MIN_ = -80;//65;

	  /****************************************************************************
	   *min magnitude of the Earth's magnetic field in units of Gauss
	   ***************************************************************************/
		const double M_MAX_ = 80;//65

public:
	  /****************************************************************************
	   * beta = 0.01 for medium movement or beta = 2.0 for slow movement
	   ***************************************************************************/
	   QuaternionGradientDescentBlock(const double beta = 2.0)
	  {
		   Ma_.zeros();
		   Mm_.zeros();
		   setBeta(beta);
	  }

	  /****************************************************************************
	   ***************************************************************************/
	  ~QuaternionGradientDescentBlock()
	  {
	  	setBeta(0);
	  }

	  /****************************************************************************
	   ***************************************************************************/
	  void setBeta(const double beta)
	  {
	  	beta_ = beta;
	  	beta_a_ = beta;
	  	beta_m_ = beta;
	  }

	  /****************************************************************************
	   ***************************************************************************/
	  double getBeta()
	  {
	  	return beta_;
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

  void convertAcc(){
	  q_conj_ = Quaternion(0,-1,0,0);
	  q_conj_.to_conj();
	  acc_old_ = acc_;
	  acc_old_ *= q_conj_;
	  q_conj_.to_conj();
	  q_conj_*=acc_old_;
	  acc_ = q_conj_;
  }

  void getMatrices(){
		Ma_(0,1) = -q_.v2();
		Ma_(0,2) = q_.v1();
		Ma_(0,3) = q_.s();
		Ma_(1,1) = q_.v3();
		Ma_(1,2) = q_.s();
		Ma_(1,3) = -q_.v1();
		Ma_(2,1) = -q_.s();
		Ma_(2,2) = q_.v3();
		Ma_(2,3) = -q_.v2();
		Ma_(3,1) = q_.v1();
		Ma_(3,2) = q_.v2();
		Ma_(3,3) = q_.v3();

		Mm_(0,1) = q_.v3();
		Mm_(0,2) = q_.s();
		Mm_(0,3) = -q_.v1();
		Mm_(1,1) = q_.v2();
		Mm_(1,2) = -q_.v1();
		Mm_(1,3) = -q_.s();
		Mm_(2,1) = q_.v1();
		Mm_(2,2) = q_.v2();
		Mm_(2,3) = q_.v3();
		Mm_(3,1) = q_.s();
		Mm_(3,2) = -q_.v3();
		Mm_(3,3) = q_.v2();
  }

  void DifferenceEquations()
  {
  	fa_[0] = q_.s()*q_.v2() - q_.v1()*q_.v3();
  	fa_[1] =-q_.s()*q_.v1() - q_.v2()*q_.v3();
  	fa_ *= 2;
  	fa_[2] = -q_.s()*q_.s() + q_.v1()*q_.v1() + q_.v2()*q_.v2() - q_.v3()*q_.v3();
  	fa_ -= acc_.v();

  	fm_[0] = -q_.s()*q_.v3() - q_.v1()*q_.v2();
  	fm_[2] = q_.s()*q_.v1() - q_.v2()*q_.v3();
  	fm_ *= 2;
  	fm_[1] = -q_.s()*q_.s() + q_.v1()*q_.v1() - q_.v2()*q_.v2() + q_.v3()*q_.v3();
  	fm_ -= mag_.v();
  }

  inline void DifferenceEquations(const Quaternion &q, const Quaternion &acc, const Quaternion &mag)
  {
	fa_[0] = q.s()*q.v2() - q.v1()*q.v3();
	fa_[1] =-q.s()*q.v1() - q.v2()*q.v3();
	fa_ *= 2;
	fa_[2] = -q.s()*q.s() + q.v1()*q.v1() + q.v2()*q.v2() - q.v3()*q.v3();
	fa_ -= acc.v();

	fm_(0) = -q.s()*q.v3() - q.v1()*q.v2();
    fm_[2] = q.s()*q.v1() - q.v2()*q.v3();
    fm_ *= 2;
    fm_[1] = -q.s()*q.s() + q.v1()*q.v1() - q.v2()*q.v2() + q.v3()*q.v3();
    fm_ -= mag.v();
  }

  inline void ArmijoRule(const Quaternion &q, const Quaternion &acc, const Quaternion &mag,
                                colvec4 &faOld, colvec4 &fmOld,
                                double &beta_acc, double &beta_mag)
  {
    q_ = q;
    acc_ = acc;
    mag_ = mag;
    faOld_ = faOld;
    fmOld_ = fmOld;

    acc_.to_normalized();
    mag_.to_normalized();
    mag_.cross(acc_);
    mag_.to_conj();
    mag_.to_normalized();

    DifferenceEquations(q_, acc_, mag_);
    faOld = fa_;
    fmOld = fm_;

    norm_fa_ = arma::norm(fa_);
    norm_fa_ -= arma::norm(faOld_);
    norm_fm_ = arma::norm(fm_);
    norm_fm_ -= arma::norm(fmOld_);

    norm_fa_ -= 0.001*beta_acc*3/8;
    norm_fm_ -= 0.001*beta_mag*3/8;

    if (norm_fa_ <= 0 && beta_acc <= 200)
    {
        beta_acc = 200;
    }
    else if(norm_fa_ > 0 && beta_acc > 1)
    {
        beta_acc /= 2;
    }
    if (norm_fm_ <= 0 && beta_mag <= 200)
    {
        beta_mag = 200;
    }
    else if(norm_fm_ > 0 && beta_mag > 1)
    {
        beta_mag /= 2;
    }

    if(arma::norm(gyro_.v()) > 0.05)
    // if(arma::norm(gyro_.v()) > 0.01)
  	{
  		beta_acc = 2;
  	  beta_mag = 2;
  	}
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
   * calculate equivalemt magnetometer measurement quaternion v_m = v_a × v_m = - v_m × v_a
   ***************************************************************************/
  void MagEquivalent()
  {
  	mag_.cross(acc_);
  	mag_.to_conj();
  	mag_.to_normalized();
  }

  /****************************************************************************
   * Implementation of the Wilson fusion algorithm
   ***************************************************************************/
	Quaternion run(const Quaternion &gyro,
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
		beta_m_ = beta_;
		beta_a_ = beta_;

//		q_norm_ = 2*q_.abs_square()*q_;
//		q_norm_ = q_.abs();
		acc_.to_normalized();
		mag_.to_normalized();

		/** upper branch **/
//		MagDistRejection();

		/** get quaternion matrices **********************/
		getMatrices();
		acc_.to_colvec(vec_acc);
		mag_.to_colvec(vec_mag);

//		/** calculate and normalize the gradient for the accelerometer *******/
		gradFa_ = Quaternion(Ma_*vec_acc + Mm_*vec_mag);
		gradFa_.to_normalized();
		gradFa_ *= beta_a_;

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
# END OF FILE
#############################################################################*/

#endif /* __QUATERNIONGRADIENTDESCENTBLOCK_H **********************************************/
