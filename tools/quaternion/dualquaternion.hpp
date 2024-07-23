#ifndef __DUALQUATERNION_H
#define __DUALQUATERNION_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include <armadillo>
#include <cmath>
#include <complex>
#include "../quaternion/quaternion.hpp"

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
 *****************************************************************************/
class DualQuaternion {

	/** 3D column vector type **************************************************/
  typedef arma::Col<double>::fixed<3> colvec;

  /** 4D column vector type **************************************************/
  typedef arma::Col<double>::fixed<4> colvec4;

  /** 8D column vector type **************************************************/
  typedef arma::Col<double>::fixed<8> colvec8;

  /** 3x3 matrix type ********************************************************/
  typedef arma::Mat<double>::fixed<3,3> mat3x3;

  /** 4x3 matrix type ********************************************************/
  typedef arma::Mat<double>::fixed<4,3> mat4x3;

  /** 4x4 matrix type ********************************************************/
  typedef arma::Mat<double>::fixed<4,4> mat4x4;

	/** type for dual quanternion states ********************************************/
	class State {
	public:

		State(double value = 0, bool is_void = true)
		{
			setValue(value);
			is_void_ = is_void;
		}

		void setValue(const double value)
		{
			value_ = value;
			is_void_ = false;
			if ( FP_ZERO == std::fpclassify(value_) )
				is_zero_ = true;
			else
				is_zero_ = false;
		}

		double value() const { return value_; }

		bool is_void_;

		bool is_zero_;

	private:

		double value_;
	};

  /** states of this quaternion **********************************************/
	State abs_;
	State abs_square_;

  /**  ****************************************/
	bool is_void_ = true;
	bool is_inverted_ = false;
	bool is_normalized_ = false;

  /** scalar part of this quanternion ****************************************/
	double sr_, sd_;

	/** column part of this quanternion ****************************************/
	colvec vr_, vd_;

	colvec4 r_, d_;

  /****************************************************************************
   * set this quaternion to void, i.e. its states are not valid
   ***************************************************************************/
	void to_void()
	{
		is_void_ = true;
		is_inverted_ = false;
		is_normalized_ = false;
		abs_.is_void_ = true;
		abs_square_.is_void_ = true;
	}

public:

  /****************************************************************************
   * construct quaternion from eight double numbers
   ***************************************************************************/
  DualQuaternion(const double sr = 0, const double vr1 = 0,
  					 const double vr2 = 0, const double vr3 = 0,
					 const double sd = 0, const double vd1 = 0,
  					 const double vd2 = 0, const double vd3 = 0)
  {
  	sr_ = sr;
  	vr_(0) = vr1;
  	vr_(1) = vr2;
  	vr_(2) = vr3;
  	sd_ = sd;
  	vd_(0) = vd1;
  	vd_(1) = vd2;
  	vd_(2) = vd3;
  }

  /****************************************************************************
   * construct quaternion from other dual quaternion
   ***************************************************************************/
  DualQuaternion(const DualQuaternion & q)
  {
  	sr_ = q.sr();
  	vr_ = q.vr();
  	sd_ = q.sd();
  	vd_ = q.vd();
  }

  /****************************************************************************
   * construct dual quaternion from other quaternion for rotation Q = q_rr + epsilon * 0 (Eq. 14 Salchow-Hömmen)
   ***************************************************************************/
  DualQuaternion(const Quaternion & q)
  {
  	sr_ = q.s();
  	vr_ = q.v();
  	sd_ = 0;
  	vd_ = {0,0,0};
  }

  /****************************************************************************
   * construct quaternion from scalar and vector parts of other dual quaternion
   ***************************************************************************/
  DualQuaternion(const double sr, const colvec & vr, const double sd, const colvec & vd)
  {
  	sr_ = sr;
  	vr_ = vr;
  	sd_ = sd;
  	vd_ = vd;
  }

  /****************************************************************************
   * construct quaternion from a 8D column vector
   ***************************************************************************/
  DualQuaternion(const colvec8 & v)
  {
  	sr_ = v(0);
  	vr_ = v.subvec(1,3);
  	sd_ = v(4);
  	vd_ = v.subvec(5,7);
  }

  /****************************************************************************
   * construct quaternion from a 4D column vector
   ***************************************************************************/
  DualQuaternion(const colvec4 & v)
  {
  	sr_ = v(0);
  	vr_ = v.subvec(1, 3);
	sd_ = 0;
	vd_ = {0,0,0};
  }

  /****************************************************************************
   * construct quaternion from a double number
   ***************************************************************************/
  DualQuaternion(const double & l)
  {
	  sr_ = 1.0;
	  vr_.zeros();
	  sd_ = 0;
	  vd_ = {0,-l*0.5 ,0};
  }

  /****************************************************************************
   ***************************************************************************/
  ~DualQuaternion()
  {

  }

  /****************************************************************************
   * accessors for scalar and vector parts of this quaternion
   ***************************************************************************/
  double sr() const { return sr_; }
  colvec vr() const { return vr_; }
  double vr1() const { return vr_(0); }
  double vr2() const { return vr_(1); }
  double vr3() const { return vr_(2); }
  double sd() const { return sd_; }
  colvec vd() const { return vd_; }
  double vd1() const { return vd_(0); }
  double vd2() const { return vd_(1); }
  double vd3() const { return vd_(2); }

  /****************************************************************************
   * accessors for complex<double> numbers of this quaternion
   ***************************************************************************/
  std::complex<double> c1() const { return {sr_, vr_(0)}; }
  std::complex<double> c2() const { return {vr_(1), vr_(2)}; }
  std::complex<double> c3() const { return {sd_, vd_(0)}; }
  std::complex<double> c4() const { return {vd_(1), vd_(2)}; }

  /****************************************************************************
   ***************************************************************************/
  bool is_inverted() const { return is_inverted_; }

  /****************************************************************************
   ***************************************************************************/
  bool is_normalized() const { return is_normalized_; }

  /****************************************************************************
   * absolute square of this quaternion
   ***************************************************************************/
  double abs_square()
  {
		if ( abs_square_.is_void_ ) {
			abs_square_.setValue( sr_ * sr_ + arma::dot(vr_, vr_) );
		}
		return abs_square_.value();
  }

 	/****************************************************************************
   * Euclidean norm of this quaternion
   ***************************************************************************/
  double abs()
  {
  	if ( abs_.is_void_ ) {
  		abs_.setValue( std::sqrt( abs_square() ) );
  	}
  	return abs_.value();
  }

  /****************************************************************************
   ***************************************************************************/
  DualQuaternion to_conj()
  {
  	vr_ = -vr_;
  	sd_ = -sd_;
  	return *this;
  }

  /****************************************************************************
   ***************************************************************************/
  DualQuaternion to_inverse()
  {
		to_conj();
		if ( ! is_normalized_ ) {
			abs_square();
			if ( ! abs_square_.is_zero_ ) {
				*this /= abs_square();
  			is_void_ = true;
				is_inverted_ = true;
  			is_normalized_ = false;
			} else {
			  /** if abs_square_ = 0 return this zero quaternion *******************/
				is_inverted_ = false;
			}
		}
		return *this;
  }

  /****************************************************************************
   ***************************************************************************/
  DualQuaternion to_normalized() //TODO
  {
  	if ( ! is_normalized_ ) {
  		abs_square();
  		if ( ! abs_square_.is_zero_ ) {
  			*this /= abs();
  			abs_.setValue(1.0);
  			abs_square_.setValue(1.0);
  			is_void_ = false;
  			is_inverted_ = false;
  			is_normalized_ = true;
  		}
  	}
  	return *this;
  }

  /****************************************************************************
   ***************************************************************************/
  DualQuaternion to_zero()
  {
  	sr_ = 0;
  	vr_.zeros();
	sd_ = 0;
	vd_.zeros();
		abs_.setValue(0.0);
		abs_square_.setValue(0.0);
		is_void_ = false;
		is_inverted_ = false;
		is_normalized_ = false;
		return *this;
  }

  /****************************************************************************
   * copy assignment operator of this quaternion
   ***************************************************************************/
  DualQuaternion & operator=(const colvec4 & v)
  {
  	sr_ = v(0);
  	vr_ = v.subvec(1, 3);
  	sd_ = 0;
  	vd_ = {0,0,0};
    return *this;
  }

  /****************************************************************************
   * copy assignment operator of this quaternion
   ***************************************************************************/
  DualQuaternion & operator=(const colvec8 & v)
  {
	  	sr_ = v(0);
	  	vr_ = v.subvec(1,3);
	  	sd_ = v(4);
	  	vd_ = v.subvec(5,7);
    return *this;
  }

  /****************************************************************************
   * copy assignment operator of this quaternion
   ***************************************************************************/
  DualQuaternion & operator=(const DualQuaternion & q) {
    sr_ = q.sr();
    vr_ = q.vr();
    sd_ = q.sd();
    vd_ = q.vd();
    return *this;
  }

  /****************************************************************************
   * copy assignment operator of this quaternion
   ***************************************************************************/
  DualQuaternion & operator=(const double & l) {
    sr_ = 1.0;
    vr_.zeros();
    sd_ = 0;
    vd_ = {0,l*0.5 ,0};
    return *this;
  }

  /****************************************************************************
   * unary operator +
   ***************************************************************************/
  DualQuaternion operator+()
  {
    return *this;
  }

  /****************************************************************************
   * unary operator -
   ***************************************************************************/
//  DualQuaternion operator-()
//  {
//    return DualQuaternion(-sr_, -vr_. -sd_, -vd_);
//  }

  /****************************************************************************
   * unary operator += for double number
   ***************************************************************************/
  DualQuaternion operator+=(const double s)
  {
    sr_ += s;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator -= for double number
   ***************************************************************************/
  DualQuaternion operator-=(const double s)
  {
    sr_ -= s;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator *= for double number
   ***************************************************************************/
  DualQuaternion operator*=(const double k)
  {
    sr_ *= k;
    vr_ *= k;
    sd_ *= k;
    vd_ *= k;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator /= for double number
   ***************************************************************************/
  DualQuaternion operator/=(const double k)
  {
    sr_ /= k;
    vr_ /= k;
    sd_ /= k;
    vd_ /= k;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator += for quaternion and 3D vector
   ***************************************************************************/
  DualQuaternion operator+=(const colvec & v)
  {
  	vr_ += v;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator -= for quaternion and 3D vector
   ***************************************************************************/
  DualQuaternion operator-=(const colvec & v)
  {
  	vr_ -= v;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator += for quaternion
   ***************************************************************************/
  DualQuaternion operator+=(const DualQuaternion & q)
  {
    sr_ += q.sr();
    vr_ += q.vr();
    sd_ += q.sd();
    vd_ += q.vd();
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator -= for quaternion
   ***************************************************************************/
  DualQuaternion operator-=(const DualQuaternion & q)
  {
    sr_ -= q.sr();
    vr_ -= q.vr();
    sd_ -= q.sd();
    vd_ -= q.vd();
    to_void();
    return *this;
  }

//  /****************************************************************************
//   * Multiplication of dual quaternions Q1 * Q2 = q1r*q2r + e(q1r*q2d + q1d*q2r) Eq. 16 Salchow-Hömmen
//   * where Q1 = q1r + e q1d and Q2 = q2r + e q2d
//   ***************************************************************************/
//  DualQuaternion operator*=(const DualQuaternion & q) //TODO
//  {
//    Quaternion q1, q2; //helping quaternions
//    //real part stays the same
//	//calc. dual part
//    q1 = Quaternion(sr_, vr_);
//    q2 = Quaternion(0, q.vr()); // q1d * q2r
//    q1 *= q2;
//    sd_ = q1.s();
//    vd_ = q1.v();
//	to_void();
//    return *this;
//  }

  /****************************************************************************
   * Multiplication of dual quaternions Q1 * Q2 = q1r*q2r + e(q1r*q2d + q1d*q2r) Eq. 16 Salchow-Hömmen
   * where Q1 = q1r + e q1d and Q2 = q2r + e q2d
   ***************************************************************************/
  DualQuaternion operator*=(const DualQuaternion & q) //TODO
  {
    Quaternion q1, q2, q3, q4, A, B; //helping quaternions
    //real part stays the same
	//calc. dual part
    q1 = Quaternion(sr_, vr_); //Q1 real part
    q2 = Quaternion(q.sr(), q.vr());// Q2 real part
    q3 = Quaternion(sd_, vd_); //Q1 dual part
    q4 = Quaternion(q.sd(), q.vd()); //Q2 dual part
    A = q1;
    A *= q2;
    sr_ = A.s();
    vr_ = A.v();
    A = q1;
    A *= q4;
    B = q3;
    B*= q2;
    A += B;
    sd_ = A.s();
    vd_ = A.v();
	to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator /= for quaternion
   * - this operator is non-commutative, i.e. p*q^-1 != q^-1*p
   * - here p /= q means p = p*q^-1, where q^-1 = conj(q) / |q|^2, i.e.
   *   p /= q is equal to p = p*conj(q) / |q|^2
   ***************************************************************************/
  DualQuaternion operator/=(DualQuaternion & q)//TODO
  {
  	double x_;

  	x_ = sr_ * q.sr() + arma::dot(vr_, q.vr());
    vr_ = q.sr() * vr_ - sr_ * q.vr() - arma::cross(vr_, q.vr());
    sr_ = x_;
    if ( ! q.is_normalized() ) {
    	*this /= q.abs_square();
    }
  	to_void();
    return *this;
  }

  /****************************************************************************
   * quaternion dot product of q_ and q, i.e. q_ = q_ ∙ q
   ***************************************************************************/
//  double dot(const DualQuaternion & q)//TODO
//  {
//    return s_ * q.s() + arma::dot(v_, q.v());
//  }


  /****************************************************************************
   * convert quaternion to Euler angles
   * This function assumes the quaternion to be normalized!
   ***************************************************************************/
  void to_EulerAngles(colvec & v)//TODO
  {
//  	v(0) = std::atan2( v_(1)*v_(2) - s_*v_(0), s_*s_ - 0.5 + v_(2)*v_(2) );
//  	v(1) = -std::asin( 2 * ( v_(0)*v_(2) + s_*v_(1) ) );
//  	v(2) = std::atan2( v_(0)*v_(1) - s_*v_(2), s_*s_ - 0.5 + v_(0)*v_(0) );
  }

  /****************************************************************************
  * Get 8x1 column vector of a quaternion
  ***************************************************************************/
  void to_colvec8(colvec8 &vec)
  {
	  vec.at(0) = sr_;
	  vec.at(1) = vr_(0);
	  vec.at(2) = vr_(1);
	  vec.at(3) = vr_(2);
	  vec.at(4) = sd_;
	  vec.at(5) = vd_(0);
	  vec.at(6) = vd_(1);
	  vec.at(7) = vd_(2);
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
# INLINE NON MEMBER DEFINITIONS
#############################################################################*/



/******************************************************************************
 * normalization of a quaternion
 *****************************************************************************/
//inline DualQuaternion normalize(const DualQuaternion & q)
//{
//	return DualQuaternion(q).to_normalized();
//}

/******************************************************************************
 * conjugate of a quaternion
 *****************************************************************************/
inline DualQuaternion conj(const DualQuaternion & q)
{
  return DualQuaternion(q).to_conj();
}

/******************************************************************************
 * operator * for a quaternion and a double number
 *****************************************************************************/
inline DualQuaternion operator*(const DualQuaternion & q, const double k)
{
  return DualQuaternion(q) *= k;
}

/******************************************************************************
 * operator * for a double number and a quaternion
 *****************************************************************************/
inline DualQuaternion operator*(const double k, const DualQuaternion & q)
{
  return DualQuaternion(q) *= k;
}

/******************************************************************************
 * operator * for two quaternions
 *****************************************************************************/
inline DualQuaternion operator*(const DualQuaternion & q1, const DualQuaternion & q2)
{
  return DualQuaternion(q1) *= q2;
}

/******************************************************************************
 * operator / for a quaternion and a double number
 *****************************************************************************/
inline DualQuaternion operator/(const DualQuaternion & q, const double k)
{
  return DualQuaternion(q) /= k;
}

/******************************************************************************
 * operator / for a double number and a quaternion
 *****************************************************************************/
//inline DualQuaternion operator/(const double k, const DualQuaternion & q)
//{
//  return DualQuaternion(q).to_inverse() *= k;
//}

/******************************************************************************
 * operator / for two quaternions
 *****************************************************************************/
inline DualQuaternion operator/(const DualQuaternion & q1, DualQuaternion & q2)
{
  return DualQuaternion(q1) /= q2;
}

/******************************************************************************
 * dot product of two quaternions
 *****************************************************************************/
//inline double dot(const DualQuaternion & q1, const DualQuaternion & q2) //TODO
//{
//	return q1.s() * q2.s() + arma::dot(q1.v(), q2.v());
//}

///******************************************************************************
// * convert a column vector of Euler angles into a quaternion
// *****************************************************************************/
//inline Quaternion fromEulerAngles(const Quaternion::colvec & v)
//{
//  return Quaternion();
//}
//
///******************************************************************************
// * convert a quaternion into a column vector of Euler angles
// *****************************************************************************/
//inline Quaternion::colvec toEulerAngles(const Quaternion & q)
//{
//  return {
//  	std::atan2( q.v2()*q.v3() - q.s()*q.v1(), q.s()*q.s() - 0.5 + q.v3() * q.v3() ),
//  	-std::asin( 2 * ( q.v1()*q.v3() + q.s()*q.v2() ) ),
//		std::atan2( q.v1()*q.v2() - q.s()*q.v3(), q.s()*q.s() - 0.5 + q.v1() * q.v1() )
//  };
//}
//
///******************************************************************************
// * convert a column vector of Euler angles into a quaternion
// *****************************************************************************/
//inline Quaternion fromRotationMatrix(const Quaternion::mat3x3 & m)
//{
//  return Quaternion();
//}
//
///******************************************************************************
// * convert a quaternion into a column vector of Euler angles
// *****************************************************************************/
//inline Quaternion::mat3x3 toRotationMatrix(const Quaternion & q)
//{
//  return { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
//}


/**############################################################################
# END OF FILE
#############################################################################*/

#endif /* __QUATERNION_H *****************************************************/
