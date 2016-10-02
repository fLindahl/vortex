#pragma once
#include <cmath>
#include "vector3.h"

#define PI 3.14159265358979323846f
using namespace std;

namespace Math
{

/// Constructor start
class Matrix3{
public:
	Matrix3();
	Matrix3(float m0, float m1, float m2,		///initiate matrix. First row.
			float m3, float m4, float m5,		///Second row
			float m6, float m7, float m8);		///Third row
	~Matrix3();
	
	void set(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8);		//Set values
	const float* get();		///Get array
	
	Vector3 operator*(const Vector3& rhs) const;	///product Mat*VectA = VectB
	Matrix3 operator*(const Matrix3& rhs) const;	///product MatA*MatB = MatC

	float       operator[](int i) const;
	float&      operator[](int i);

	Matrix3 identity();
	Matrix3 transpose();	/// Transpose Function.
	Matrix3 invert();		/// Invert Function

	///Init rotational matrices
	static Matrix3 rotX(float angle);
	static Matrix3 rotY(float angle);
	static Matrix3 rotZ(float angle);
	static Matrix3 rotVector(float angle, const Vector3& vec);

	friend Matrix3 operator*(float scalar, const Matrix3& m);		///Scalar product
	friend Vector3 operator*(const Vector3& vec, const Matrix3& m);	///Vector Product

private:
	float m[9];
};
/// end of constructor

inline Matrix3::Matrix3(){
	identity(); ///Create Identity-matrix if no arguments are provided
}
inline Matrix3::Matrix3(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8){
	set(m0, m1, m2, m3, m4, m5, m6, m7, m8); ///Set private array from arguments
}

inline Matrix3::~Matrix3(){

}

inline void Matrix3::set(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8){
	m[0] = m0;
	m[1] = m1;
	m[2] = m2;
	m[3] = m3;
	m[4] = m4;
	m[5] = m5;
	m[6] = m6;
	m[7] = m7;
	m[8] = m8;
}

inline const float* Matrix3::get(){
	return m;
}

inline Vector3 Matrix3::operator*(const Vector3& rhs) const{
	return Vector3( m[0] * rhs[0] + m[3] * rhs[1] + m[6] * rhs[2],
					m[1] * rhs[0] + m[4] * rhs[1] + m[7] * rhs[2],
					m[2] * rhs[0] + m[5] * rhs[1] + m[8] * rhs[2] );
}

inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const{
	return Matrix3( m[0] * rhs[0] + m[3] * rhs[1] + m[6] * rhs[2], m[1] * rhs[0] + m[4] * rhs[1] + m[7] * rhs[2], m[2] * rhs[0] + m[5] * rhs[1] + m[8] * rhs[2],
					m[0] * rhs[3] + m[3] * rhs[4] + m[6] * rhs[5], m[1] * rhs[3] + m[4] * rhs[4] + m[7] * rhs[5], m[2] * rhs[3] + m[5] * rhs[4] + m[8] * rhs[5],
					m[0] * rhs[6] + m[3] * rhs[7] + m[6] * rhs[8], m[1] * rhs[6] + m[4] * rhs[7] + m[7] * rhs[8], m[2] * rhs[6] + m[5] * rhs[7] + m[8] * rhs[8] );
}

inline float Matrix3::operator[](int i) const{
	return m[i];
}

inline float& Matrix3::operator[](int i){
	return m[i];
}

inline Matrix3 Matrix3::transpose(){ 
	return Matrix3(m[0], m[3], m[6], m[1], m[4], m[7], m[2], m[5], m[8]);
}

inline Matrix3 Matrix3::identity(){
	return Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

inline Matrix3 Matrix3::invert(){

	float det;
	float invDet;

	float temp[9];


	temp[0] = m[4] * m[8] - m[5] * m[7];
	temp[1] = m[2] * m[7] - m[1] * m[8];
	temp[2] = m[1] * m[5] - m[2] * m[4];
	temp[3] = m[5] * m[6] - m[3] * m[8];
	temp[4] = m[0] * m[8] - m[2] * m[6];
	temp[5] = m[2] * m[3] - m[0] * m[5];
	temp[6] = m[3] * m[7] - m[4] * m[6];
	temp[7] = m[1] * m[6] - m[0] * m[7];
	temp[8] = m[0] * m[4] - m[1] * m[3];

	/// check determinant if it is 0
	det = m[0] * temp[0] + m[1] * temp[3] + m[2] * temp[6];
	if (fabs(det) <= 0.00001f) ///Epsilon
	{
		return identity(); /// cannot inverse, make it identity matrix
	}

	/// Divide with determinatn
	invDet = 1.0f / det;
	
	return Matrix3( invDet * temp[0], invDet * temp[1], invDet * temp[2],
					invDet * temp[3], invDet * temp[4], invDet * temp[5],
					invDet * temp[6], invDet * temp[7], invDet * temp[8] );
}


inline Matrix3 Matrix3::rotX(float angle){

	float result;
	float c;
	float s;

	if (angle == 180.0f){
		result = PI;
		s = 0;
	}
	else{
		result = (angle*PI / 180.0f);
		s = sin(result);
	}

	c = cos(result);

	return Matrix3( 1, 0, 0,
					0, c, -s,
					0, s, c );
}

inline Matrix3 Matrix3::rotY(float angle){

	float result;
	float c;
	float s;

	if (angle == 180.0f){
		result = PI;
		s = 0;
	}
	else{
		result = (angle*PI / 180.0f);
		s = sin(result);
	}

	c = cos(result);

	return Matrix3( c, 0, s,
					0, 1, 0,
					-s, 0, c );
}

inline Matrix3 Matrix3::rotZ(float angle){

	float result;
	float c;
	float s;

	if (angle == 180.0f){
		result = PI;
		s = 0;
	}
	else{
		result = (angle*PI / 180.0f);
		s = sin(result);
	}

	c = cos(result);

	return Matrix3( c, -s, 0,
					s, c, 0,
					0, 0, 1 );
}

///Rotate around vector.
inline Matrix3 Matrix3::rotVector(float angle, const Vector3& vec){ 

	float x = vec[0], y = vec[1], z = vec[2];
	float result;
	float c;
	float s;

	if (angle == 180.0f){
		result = PI;
		s = 0;
	}
	else{
		result = (angle*PI / 180.0f);
		s = sin(result);
	}

	c = cos(result);
	
	return Matrix3( ///TODO: Make less cluttered.
		(x*x) + ((1 - (x*x))*c), (x*y*(1 - c)) - (x*s), (x*z*(1-c))+(y*s),
		(x*z*(1 - c)) + (z*s), (y*y*(1 - (y*y))*c), (x*z*(1-c))-(x*s),
		(x*z*(1 - c)) - (y*s), (y*z*(1 - c)) + (x*s), (z*z) + ((1-(z*z))*c)
		);
}

///Friend functions
inline Matrix3 operator*(float scalar, const Matrix3& rhs){
	return Matrix3(scalar*rhs[0], scalar*rhs[1], scalar*rhs[2], scalar*rhs[3], scalar*rhs[4], scalar*rhs[5], scalar*rhs[6], scalar*rhs[7], scalar*rhs[8] );
}

inline Vector3 operator*(const Vector3& vec, const Matrix3& mat){
	return Vector3(vec[0]*mat[0] + vec[1]*mat[1] + vec[2]*mat[2], vec[0]*mat[3] + vec[1]*mat[4] + vec[2]*mat[5], vec[0]*mat[6] + vec[1]*mat[7] + vec[2]*mat[8] );
}


}