#ifndef __QUATERNION_H
#define __QUATERNION_H

/**############################################################################
# INCLUDES
#############################################################################*/

#include <armadillo>
#include <cmath>
#include <complex>

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
class Quaternion {

	/** 3D column vector type **************************************************/
  typedef arma::Col<double>::fixed<3> colvec;

  /** 4D column vector type **************************************************/
  typedef arma::Col<double>::fixed<4> colvec4;

  /** 3x3 matrix type ********************************************************/
  typedef arma::Mat<double>::fixed<3,3> mat3x3;

  /** 4x3 matrix type ********************************************************/
  typedef arma::Mat<double>::fixed<4,3> mat4x3;

  /** 4x4 matrix type ********************************************************/
  typedef arma::Mat<double>::fixed<4,4> mat4x4;

	/** type for quanternion states ********************************************/
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
	double s_;

	/** column part of this quanternion ****************************************/
	colvec v_;
	colvec4 vec4_;

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
   * construct quaternion from four double numbers
   ***************************************************************************/
  Quaternion(const double s = 0, const double v1 = 0,
  					 const double v2 = 0, const double v3 = 0)
  {
  	s_ = s;
  	v_(0) = v1;
  	v_(1) = v2;
  	v_(2) = v3;
  }

  /****************************************************************************
   * construct quaternion from one or two complex<double> numbers
   ***************************************************************************/
  Quaternion(const std::complex<double> & c1,
  					 const std::complex<double> & c2 = std::complex<double>(0, 0))
  {
   	s_ = c1.real();
  	v_(0) = c1.imag();
  	v_(1) = c2.real();
  	v_(2) = c2.imag();
  }

  /****************************************************************************
   * construct quaternion from other quaternion
   ***************************************************************************/
  Quaternion(const Quaternion & q)
  {
  	s_ = q.s();
  	v_ = q.v();
  }

  /****************************************************************************
   * construct quaternion from scalar and vector parts of other quaternion
   ***************************************************************************/
  Quaternion(const double s, const colvec & v)
  {
  	s_ = s;
  	v_ = v;
  }

  /****************************************************************************
   * construct quaternion from scalar and vector parts of other quaternion
   ***************************************************************************/
  Quaternion(const colvec4 & v)
  {
  	s_ = v(0);
  	v_ = v.subvec(1, 3);
  }

  /****************************************************************************
   * construct quaternion from axis and angle
   ***************************************************************************/
  Quaternion(const colvec & v, const double theta)
  {
  	s_ = cos(theta/2);
  	v_ = v*sin(theta/2);
  }

  /****************************************************************************
   ***************************************************************************/
  ~Quaternion()
  {

  }

  /****************************************************************************
   * accessors for scalar and vector parts of this quaternion
   ***************************************************************************/
  double s() const { return s_; }
  colvec v() const { return v_; }
  double v1() const { return v_(0); }
  double v2() const { return v_(1); }
  double v3() const { return v_(2); }

  /****************************************************************************
   * accessors for complex<double> numbers of this quaternion
   ***************************************************************************/
  std::complex<double> c1() const { return {s_, v_(0)}; }
  std::complex<double> c2() const { return {v_(1), v_(2)}; }

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
			abs_square_.setValue( s_ * s_ + arma::dot(v_, v_) );
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
	 * Euclidean norm of this quaternion
 	 ***************************************************************************/
	double norm()
	{
		double norm = s_*s_ + arma::norm(v_);
		return norm;
	}

  /****************************************************************************
   ***************************************************************************/
  Quaternion to_conj()
  {
  	v_ = -v_;
  	return *this;
  }

  /****************************************************************************
   ***************************************************************************/
  Quaternion to_inverse()
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
  Quaternion to_normalized()
  {
  	if ( ! is_normalized_ ) {
  		abs_square();
  		if ( ! abs_square_.is_zero_ ) {
  			*this /= abs();
//  			abs_.setValue(1.0);
//  			abs_square_.setValue(1.0);
//  			is_void_ = false;
//  			is_inverted_ = false;
//  			is_normalized_ = true;
  		}
  	}
  	return *this;
  }

  /****************************************************************************
   ***************************************************************************/
  Quaternion to_zero()
  {
  	s_ = 0;
  	v_.zeros();
		abs_.setValue(0.0);
		abs_square_.setValue(0.0);
		is_void_ = false;
		is_inverted_ = false;
		is_normalized_ = false;
		return *this;
  }

  Quaternion rotationTo(colvec4 &fromAxis, colvec4 &toAxis)
  {
    Quaternion q;
    colvec4 a;
    a = arma::cross(fromAxis, toAxis);
    v_ = a;
    double x = arma::norm(fromAxis);
    double y = arma::norm(toAxis);

    s_ = sqrt((x * x) * (y * y)) + arma::dot(fromAxis, toAxis);
    q = Quaternion(s_, v_);
    q.to_normalized();
    return q;
  }

  /****************************************************************************
   * copy assignment operator of this quaternion
   ***************************************************************************/
  Quaternion & operator=(const colvec4 & v)
  {
  	s_ = v(0);
  	v_ = v.subvec(1, 3);
    return *this;
  }

  /****************************************************************************
   * copy assignment operator of this quaternion
   ***************************************************************************/
  Quaternion & operator=(const Quaternion & q) {
    s_ = q.s();
    v_ = q.v();
    return *this;
  }

  /****************************************************************************
   * unary operator +
   ***************************************************************************/
  Quaternion operator+()
  {
    return *this;
  }

  /****************************************************************************
   * unary operator -
   ***************************************************************************/
  Quaternion operator-()
  {
    return Quaternion(-s_, -v_);
  }

  /****************************************************************************
   * unary operator += for double number
   ***************************************************************************/
  Quaternion operator+=(const double s)
  {
    s_ += s;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator -= for double number
   ***************************************************************************/
  Quaternion operator-=(const double s)
  {
    s_ -= s;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator *= for double number
   ***************************************************************************/
  Quaternion operator*=(const double k)
  {
    s_ *= k;
    v_ *= k;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator /= for double number
   ***************************************************************************/
  Quaternion operator/=(const double k)
  {
    s_ /= k;
    v_ /= k;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator += for quaternion and 3D vector
   ***************************************************************************/
  Quaternion operator+=(const colvec & v)
  {
  	v_ += v;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator -= for quaternion and 3D vector
   ***************************************************************************/
  Quaternion operator-=(const colvec & v)
  {
  	v_ -= v;
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator += for quaternion
   ***************************************************************************/
  Quaternion operator+=(const Quaternion & q)
  {
    s_ += q.s();
    v_ += q.v();
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator -= for quaternion
   ***************************************************************************/
  Quaternion operator-=(const Quaternion & q)
  {
    s_ -= q.s();
    v_ -= q.v();
    to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator *= for quaternion
   * this is known as the Graßmann product p*q, see
   * https://mathepedia.de/Quaternionen.html
   ***************************************************************************/
  Quaternion operator*=(const Quaternion & q)
  {
    double x_;
    x_ = s_ * q.s() - arma::dot(v_, q.v());
    v_ = s_ * q.v() + q.s() * v_ + arma::cross(v_, q.v());
    s_ = x_;
  	to_void();
    return *this;
  }

  /****************************************************************************
   * unary operator /= for quaternion
   * - this operator is non-commutative, i.e. p*q^-1 != q^-1*p
   * - here p /= q means p = p*q^-1, where q^-1 = conj(q) / |q|^2, i.e.
   *   p /= q is equal to p = p*conj(q) / |q|^2
   ***************************************************************************/
  Quaternion operator/=(Quaternion & q)
  {
  	double x_;

  	x_ = s_ * q.s() + arma::dot(v_, q.v());
    v_ = q.s() * v_ - s_ * q.v() - arma::cross(v_, q.v());
    s_ = x_;
    if ( ! q.is_normalized() ) {
    	*this /= q.abs_square();
    }
  	to_void();
    return *this;
  }

  /****************************************************************************
   * quaternion dot product of q_ and q, i.e. q_ = q_ ∙ q
   ***************************************************************************/
  double dot(const Quaternion & q)
  {
    return s_ * q.s() + arma::dot(v_, q.v());
  }

  /****************************************************************************
   * quaternion cross product of q_ and q, i.e. q_ = q_ × q
   ***************************************************************************/
  void cross(const Quaternion & q)
  {
    s_ = 0;
    v_ = arma::cross(v_, q.v());
    to_void();
  }

  /****************************************************************************
   * convert quaternion to Euler angles
   * This function assumes the quaternion to be normalized!
   ***************************************************************************/
  void to_EulerAngles(colvec & v)
  {
  	v(0) = std::atan2( v_(1)*v_(2) - s_*v_(0), s_*s_ - 0.5 + v_(2)*v_(2) );
  	v(1) = -std::asin( 2 * ( v_(0)*v_(2) + s_*v_(1) ) );
  	v(2) = std::atan2( v_(0)*v_(1) - s_*v_(2), s_*s_ - 0.5 + v_(0)*v_(0) );
  }

  /****************************************************************************
  * Get 4x1 column vector of a quaternion
  ***************************************************************************/
  void to_colvec(colvec4 &vec)
  {
	  vec.at(0) = s_;
	  vec.at(1) = v_(0);
	  vec.at(2) = v_(1);
	  vec.at(3) = v_(2);
  }

  /****************************************************************************
   * convert Euler angles to quaternion
   ***************************************************************************/
  void to_quaternion(const colvec &v) //TODO how to pass Euler angles as v
  {
  	double cr = std::cos(v(0) * 0.5);
  	double cp = std::cos(v(1) * 0.5);
  	double cy = std::cos(v(2) * 0.5);
  	double sr = std::sin(v(0) * 0.5);
  	double sp = std::sin(v(1) * 0.5);
  	double sy = std::sin(v(2) * 0.5);
  	s_= cr * cp * cy + sr * sp * sy;
  	v_(0) = sr * cp * cy - cr * sp * sy;
  	v_(1) = cr * sp * cy + sr * cp * sy;
  	v_(2) = cr * cp * sy - sr * sp * cy;
  }

  /****************************************************************************
   * convert quaternion to rotation matrix
   ***************************************************************************/
  void to_rotationMatrix(mat3x3 &mat)
  {
	  mat3x3 skew, eye;
	  skew.zeros();
	  skew.at(1,0) = v_(2);
	  skew.at(0,1) = -v_(2);
	  skew.at(2,1) = v_(0);
	  skew.at(1,2) = -v_(0);
	  skew.at(0,2) = v_(1);
	  skew.at(2,0) = -v_(1);
	  eye.eye();
	  mat= v_*v_.t() + s_*s_*eye + 2 * s_*skew + skew*skew;
  }

  //** quaternion rotation of a vector; convert vector from frame A to frame B **//
  void convertFrame(Quaternion &vec, Quaternion &q){
    Quaternion q_help_;
	  q_help_ = q;
 	  q.to_conj();
	  vec *= q;
	  q_help_ *= vec;
    vec = q_help_;
  }

  void rotate_vector_by_quaternion(const arma::vec& v, const Quaternion& q, arma::vec& vprime)
  {
      // Extract the vector part of the quaternion
	  arma::vec u;
	  u = (q.v1(), q.v2(), q.v3());

      // Extract the scalar part of the quaternion
      float s = q.s();

      // Do the math
      vprime = 2.0 * arma::dot(u, v) * u
            + (s*s - arma::dot(u, u)) * v
            + 2.0f * s * arma::cross(u, v);
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
 * absolute square of a quaternion
 *****************************************************************************/
inline double abs_square(Quaternion & q)
{
  return q.abs_square();
}

/******************************************************************************
 * Euclidean norm of a quaternion
 *****************************************************************************/
inline double abs(Quaternion & q)
{
  return q.abs();
}

/******************************************************************************
 * normalization of a quaternion
 *****************************************************************************/
inline Quaternion normalize(const Quaternion & q)
{
	return Quaternion(q).to_normalized();
}

/******************************************************************************
 * conjugate of a quaternion
 *****************************************************************************/
inline Quaternion conj(const Quaternion & q)
{
  return Quaternion(q).to_conj();
}

/******************************************************************************
 * inverse of a quaternion
 *****************************************************************************/
inline Quaternion inverse(const Quaternion & q)
{
	return Quaternion(q).to_inverse();
}

/******************************************************************************
 * operator * for a quaternion and a double number
 *****************************************************************************/
inline Quaternion operator*(const Quaternion & q, const double k)
{
  return Quaternion(q) *= k;
}

/******************************************************************************
 * operator * for a double number and a quaternion
 *****************************************************************************/
inline Quaternion operator*(const double k, const Quaternion & q)
{
  return Quaternion(q) *= k;
}

/******************************************************************************
 * operator * for two quaternions
 *****************************************************************************/
inline Quaternion operator*(const Quaternion & q1, const Quaternion & q2)
{
  return Quaternion(q1) *= q2;
}

/******************************************************************************
 * operator / for a quaternion and a double number
 *****************************************************************************/
inline Quaternion operator/(const Quaternion & q, const double k)
{
  return Quaternion(q) /= k;
}

/******************************************************************************
 * operator / for a double number and a quaternion
 *****************************************************************************/
inline Quaternion operator/(const double k, const Quaternion & q)
{
  return Quaternion(q).to_inverse() *= k;
}

/******************************************************************************
 * operator / for two quaternions
 *****************************************************************************/
inline Quaternion operator/(const Quaternion & q1, Quaternion & q2)
{
  return Quaternion(q1) /= q2;
}

/******************************************************************************
 * dot product of two quaternions
 *****************************************************************************/
inline double dot(const Quaternion & q1, const Quaternion & q2)
{
	return q1.s() * q2.s() + arma::dot(q1.v(), q2.v());
}

/******************************************************************************
 * cross product of two quaternions
 *****************************************************************************/
inline Quaternion cross(const Quaternion & q1, const Quaternion & q2)
{
  return Quaternion(0, arma::cross(q1.v(), q2.v()));
}

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
