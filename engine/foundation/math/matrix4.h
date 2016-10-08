#pragma once
#include "config.h"
#include <cmath>
#include "vector4.h"
#include "matrix3.h"

#define PI  3.14159265358979323846f

//#pragma pack(push, 1)

namespace Math
{

class Quaternion;

/// Constructor start
class Matrix4{
public:
	///Initiate identity matrix
	Matrix4();

	///initiate 4D Matrix with all inputs.
	Matrix4(float m0, float m1, float m2, float m3,	
			float m4, float m5, float m6, float m7,	
			float m8, float m9, float m10, float m11,
			float m12, float m13, float m14, float m15);

	///Initiate homogenous 4d matrix.
	Matrix4(float m0, float m1, float m2,	///take only the 9 first variables since we only want to initiate those in an homogenous 4D Matrix.
			float m4, float m5, float m6,	///Notice the names too.
			float m8, float m9,	float m10);

	~Matrix4();

	void set(float m0, float m1, float m2, float m4, float m5, float m6, float m8, float m9, float m10, float m3, float m7, float m11, float m12, float m13, float m14, float m15);		///Set values
	const float* get();		///Get array
	Matrix3 get3x3();		///Get array

	Vector4 operator*(const Vector4& rhs) const;	///product Mat*VectA = VectB
	Matrix4 operator*(const Matrix4& rhs) const;	///product MatA*MatB = MatC
	Matrix4 operator+(const Matrix4& rhs) const;	///addition MatA+MatB = MatC

	float operator[](const int& i) const;

	static Matrix4 identity();	/// returns an identity matrix
	Matrix4 transpose() const;	/// Transpose Function
	Matrix4 invert() const;		/// Invert Function 
	static Matrix4 inverse(const Matrix4& mat);		/// Inverse

	///Init rotational matrices
	static Matrix4 rotX(const float& angle);
	static Matrix4 rotY(const float& angle);
	static Matrix4 rotZ(const float& angle);
	static Matrix4 rotVector(const float& angle, const Vector4& vec);
	static Matrix4 translation(const float& x, const float& y, const float& z);
	static Matrix4 translation(const Vector4& vec);
	static Matrix4 scale(const float& x, const float& y, const float& z);
	static Matrix4 scale(const Vector4& v);
	
	Vector4 getPosition();

	static Matrix4 perspectiveMatrix(const float& farZ, const float& nearZ, const float& aspect, const float& fov);
	static Matrix4 viewMatrix(const float& pitch, const float& yaw, const Vector3& camerapos);
	friend Matrix4 operator*(float scalar, const Matrix4& m);		///Scalar product
	friend Vector4 operator*(const Vector4& vec, const Matrix4& m);	///Vector Product

	static Matrix4 RotationQuaternion(const Quaternion& q);
	
private:
	float m[16];
};
/// end of constructor

inline Matrix4::Matrix4(){
	///Create Identity-matrix if no arguments are provided
	set(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix4::Matrix4(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9, float m10, float m11, float m12, float m13, float m14, float m15){
	set(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15); ///Set private array from arguments

}

inline Matrix4::Matrix4(float m0, float m1, float m2, float m4, float m5, float m6, float m8, float m9, float m10){
	float m3 = 0;
	float m7 = 0;
	float m11 = 0;
	float m12 = 0;
	float m13 = 0;
	float m14 = 0;
	float m15 = 1;

	set(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15); ///Set private array from arguments
}
inline Matrix4::~Matrix4(){

}

inline void Matrix4::set(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9, float m10, float m11, float m12, float m13, float m14, float m15){
	m[0] = m0;
	m[1] = m1;
	m[2] = m2;
	m[3] = m3;
	m[4] = m4;
	m[5] = m5;
	m[6] = m6;
	m[7] = m7;
	m[8] = m8;
	m[9] = m9;
	m[10] = m10;
	m[11] = m11;
	m[12] = m12;
	m[13] = m13;
	m[14] = m14;
	m[15] = m15;
}

inline const float* Matrix4::get(){
	return m;
}

inline Matrix3 Matrix4::get3x3()
{
	return Matrix3(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);
}

inline Vector4 Matrix4::operator*(const Vector4& rhs) const{
		return Vector4(rhs[0] * m[0] + rhs[1] * m[1] + rhs[2] * m[2] + rhs[3] * m[3], rhs[0] * m[4] + rhs[1] * m[5] + rhs[2] * m[6] + rhs[3] * m[7], rhs[0] * m[8] + rhs[1] * m[9] + rhs[2] * m[10] + rhs[3] * m[11], rhs[0] * m[12] + rhs[1] * m[13] + rhs[2] * m[14] + rhs[3] * m[15]);
}


inline Matrix4 Matrix4::operator*(const Matrix4& rhs) const{
	return Matrix4( m[0] * rhs[0] + m[4] * rhs[1] + m[8] * rhs[2] + m[12] * rhs[3], m[1] * rhs[0] + m[5] * rhs[1] + m[9] * rhs[2] + m[13] * rhs[3], m[2] * rhs[0] + m[6] * rhs[1] + m[10] * rhs[2] + m[14] * rhs[3], m[3] * rhs[0] + m[7] * rhs[1] + m[11] * rhs[2] + m[15] * rhs[3],
					m[0] * rhs[4] + m[4] * rhs[5] + m[8] * rhs[6] + m[12] * rhs[7], m[1] * rhs[4] + m[5] * rhs[5] + m[9] * rhs[6] + m[13] * rhs[7], m[2] * rhs[4] + m[6] * rhs[5] + m[10] * rhs[6] + m[14] * rhs[7], m[3] * rhs[4] + m[7] * rhs[5] + m[11] * rhs[6] + m[15] * rhs[7],
					m[0] * rhs[8] + m[4] * rhs[9] + m[8] * rhs[10] + m[12] * rhs[11], m[1] * rhs[8] + m[5] * rhs[9] + m[9] * rhs[10] + m[13] * rhs[11], m[2] * rhs[8] + m[6] * rhs[9] + m[10] * rhs[10] + m[14] * rhs[11], m[3] * rhs[8] + m[7] * rhs[9] + m[11] * rhs[10] + m[15] * rhs[11],
					m[0] * rhs[12] + m[4] * rhs[13] + m[8] * rhs[14] + m[12] * rhs[15], m[1] * rhs[12] + m[5] * rhs[13] + m[9] * rhs[14] + m[13] * rhs[15], m[2] * rhs[12] + m[6] * rhs[13] + m[10] * rhs[14] + m[14] * rhs[15], m[3] * rhs[12] + m[7] * rhs[13] + m[11] * rhs[14] + m[15] * rhs[15]);
}

inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const{
	return Matrix4( m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3], 
					m[4] + rhs[4], m[5] + rhs[5], m[6] + rhs[6], m[7] + rhs[7], 
					m[8] + rhs[8], m[9] + rhs[9], m[10] + rhs[10], m[11] + rhs[11],
					m[12] + rhs[12], m[13] + rhs[13], m[14] + rhs[14], m[15] + rhs[15] );
}

inline float Matrix4::operator[](const int& i) const
{
	return m[i];
}

inline Matrix4 Matrix4::transpose() const{
	return Matrix4(m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
}

inline Matrix4 Matrix4::identity()
{
	return Matrix4( 1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1	);
}

inline Matrix4 Matrix4::invert() const{

	float det;
	float invDet;

	float temp[16];

	temp[0] = m[6]*m[11]*m[13] - m[7]*m[10]*m[13] + m[7]*m[9]*m[14] - m[5]*m[11]*m[14] - m[6]*m[9]*m[15] + m[5]*m[10]*m[15];
	temp[1] = m[3]*m[10]*m[13] - m[2]*m[11]*m[13] - m[3]*m[9]*m[14] + m[1]*m[11]*m[14] + m[2]*m[9]*m[15] - m[1]*m[10]*m[15];
	temp[2] = m[2]*m[7]*m[13] - m[3]*m[6]*m[13] + m[3]*m[5]*m[14] - m[1]*m[7]*m[14] - m[2]*m[5]*m[15] + m[1]*m[6]*m[15];
	temp[3] = m[3]*m[6]*m[9] - m[2]*m[7]*m[9] - m[3]*m[5]*m[10] + m[1]*m[7]*m[10] + m[2]*m[5]*m[11] - m[1]*m[6]*m[11];
	temp[4] = m[7]*m[10]*m[12] - m[6]*m[11]*m[12] - m[7]*m[8]*m[14] + m[4]*m[11]*m[14] + m[6]*m[8]*m[15] - m[4]*m[10]*m[15];
	temp[5] = m[2]*m[11]*m[12] - m[3]*m[10]*m[12] + m[3]*m[8]*m[14] - m[0]*m[11]*m[14] - m[2]*m[8]*m[15] + m[0]*m[10]*m[15];
	temp[6] = m[3]*m[6]*m[12] - m[2]*m[7]*m[12] - m[3]*m[4]*m[14] + m[0]*m[7]*m[14] + m[2]*m[4]*m[15] - m[0]*m[6]*m[15];
	temp[7] = m[2]*m[7]*m[8] - m[3]*m[6]*m[8] + m[3]*m[4]*m[10] - m[0]*m[7]*m[10] - m[2]*m[4]*m[11] + m[0]*m[6]*m[11];
	temp[8] = m[5]*m[11]*m[12] - m[7]*m[9]*m[12] + m[7]*m[8]*m[13] - m[4]*m[11]*m[13] - m[5]*m[8]*m[15] + m[4]*m[9]*m[15];
	temp[9] = m[3]*m[9]*m[12] - m[1]*m[11]*m[12] - m[3]*m[8]*m[13] + m[0]*m[11]*m[13] + m[1]*m[8]*m[15] - m[0]*m[9]*m[15];
	temp[10] = m[1]*m[7]*m[12] - m[3]*m[5]*m[12] + m[3]*m[4]*m[13] - m[0]*m[7]*m[13] - m[1]*m[4]*m[15] + m[0]*m[5]*m[15];
	temp[11] = m[3]*m[5]*m[8] - m[1]*m[7]*m[8] - m[3]*m[4]*m[9] + m[0]*m[7]*m[9] + m[1]*m[4]*m[11] - m[0]*m[5]*m[11];
	temp[12] = m[6]*m[9]*m[12] - m[5]*m[10]*m[12] - m[6]*m[8]*m[13] + m[4]*m[10]*m[13] + m[5]*m[8]*m[14] - m[4]*m[9]*m[14];
	temp[13] = m[1]*m[10]*m[12] - m[2]*m[9]*m[12] + m[2]*m[8]*m[13] - m[0]*m[10]*m[13] - m[1]*m[8]*m[14] + m[0]*m[9]*m[14];
	temp[14] = m[2]*m[5]*m[12] - m[1]*m[6]*m[12] - m[2]*m[4]*m[13] + m[0]*m[6]*m[13] + m[1]*m[4]*m[14] - m[0]*m[5]*m[14];
	temp[15] = m[1]*m[6]*m[8] - m[2]*m[5]*m[8] + m[2]*m[4]*m[9] - m[0]*m[6]*m[9] - m[1]*m[4]*m[10] + m[0]*m[5]*m[10];


	/// check determinant if it is 0
	det = m[3]*m[6]*m[9]*m[12] - m[2]*m[7]*m[9]*m[12] - m[3]*m[5]*m[10]*m[12] + m[1]*m[7]*m[10]*m[12] +
		  m[2]*m[5]*m[11]*m[12] - m[1]*m[6]*m[11]*m[12] - m[3]*m[6]*m[8]*m[13] + m[2]*m[7]*m[8]*m[13] +
		  m[3]*m[4]*m[10]*m[13] - m[0]*m[7]*m[10]*m[13] - m[2]*m[4]*m[11]*m[13] + m[0]*m[6]*m[11]*m[13] +
		  m[3]*m[5]*m[8]*m[14] - m[1]*m[7]*m[8]*m[14] - m[3]*m[4]*m[9]*m[14] + m[0]*m[7]*m[9]*m[14] +
		  m[1]*m[4]*m[11]*m[14] - m[0]*m[5]*m[11]*m[14] - m[2]*m[5]*m[8]*m[15] + m[1]*m[6]*m[8]*m[15] +
		  m[2]*m[4]*m[9]*m[15] - m[0]*m[6]*m[9]*m[15] - m[1]*m[4]*m[10]*m[15] + m[0]*m[5]*m[10]*m[15];
	if (fabs(det) <= 0.00001f) ///Epsilon
	{
		return identity(); /// cannot inverse, make it identity matrix
	}

	/// Divide with determinatn
	invDet = 1.0f / det;
	
	return Matrix4( invDet * temp[0], invDet * temp[1], invDet * temp[2], invDet * temp[3],
					invDet * temp[4], invDet * temp[5], invDet * temp[6], invDet * temp[7],
					invDet * temp[8], invDet * temp[9], invDet * temp[10], invDet * temp[11],
					invDet * temp[12], invDet * temp[13], invDet * temp[14], invDet * temp[15]);
}

inline Matrix4 Matrix4::inverse(const Matrix4& mat){

	float det;
	float invDet;

	float temp[16];

	temp[0] = mat[6] * mat[11] * mat[13] - mat[7] * mat[10] * mat[13] + mat[7] * mat[9] * mat[14] - mat[5] * mat[11] * mat[14] - mat[6] * mat[9] * mat[15] + mat[5] * mat[10] * mat[15];
	temp[1] = mat[3] * mat[10] * mat[13] - mat[2] * mat[11] * mat[13] - mat[3] * mat[9] * mat[14] + mat[1] * mat[11] * mat[14] + mat[2] * mat[9] * mat[15] - mat[1] * mat[10] * mat[15];
	temp[2] = mat[2] * mat[7] * mat[13] - mat[3] * mat[6] * mat[13] + mat[3] * mat[5] * mat[14] - mat[1] * mat[7] * mat[14] - mat[2] * mat[5] * mat[15] + mat[1] * mat[6] * mat[15];
	temp[3] = mat[3] * mat[6] * mat[9] - mat[2] * mat[7] * mat[9] - mat[3] * mat[5] * mat[10] + mat[1] * mat[7] * mat[10] + mat[2] * mat[5] * mat[11] - mat[1] * mat[6] * mat[11];
	temp[4] = mat[7] * mat[10] * mat[12] - mat[6] * mat[11] * mat[12] - mat[7] * mat[8] * mat[14] + mat[4] * mat[11] * mat[14] + mat[6] * mat[8] * mat[15] - mat[4] * mat[10] * mat[15];
	temp[5] = mat[2] * mat[11] * mat[12] - mat[3] * mat[10] * mat[12] + mat[3] * mat[8] * mat[14] - mat[0] * mat[11] * mat[14] - mat[2] * mat[8] * mat[15] + mat[0] * mat[10] * mat[15];
	temp[6] = mat[3] * mat[6] * mat[12] - mat[2] * mat[7] * mat[12] - mat[3] * mat[4] * mat[14] + mat[0] * mat[7] * mat[14] + mat[2] * mat[4] * mat[15] - mat[0] * mat[6] * mat[15];
	temp[7] = mat[2] * mat[7] * mat[8] - mat[3] * mat[6] * mat[8] + mat[3] * mat[4] * mat[10] - mat[0] * mat[7] * mat[10] - mat[2] * mat[4] * mat[11] + mat[0] * mat[6] * mat[11];
	temp[8] = mat[5] * mat[11] * mat[12] - mat[7] * mat[9] * mat[12] + mat[7] * mat[8] * mat[13] - mat[4] * mat[11] * mat[13] - mat[5] * mat[8] * mat[15] + mat[4] * mat[9] * mat[15];
	temp[9] = mat[3] * mat[9] * mat[12] - mat[1] * mat[11] * mat[12] - mat[3] * mat[8] * mat[13] + mat[0] * mat[11] * mat[13] + mat[1] * mat[8] * mat[15] - mat[0] * mat[9] * mat[15];
	temp[10] = mat[1] * mat[7] * mat[12] - mat[3] * mat[5] * mat[12] + mat[3] * mat[4] * mat[13] - mat[0] * mat[7] * mat[13] - mat[1] * mat[4] * mat[15] + mat[0] * mat[5] * mat[15];
	temp[11] = mat[3] * mat[5] * mat[8] - mat[1] * mat[7] * mat[8] - mat[3] * mat[4] * mat[9] + mat[0] * mat[7] * mat[9] + mat[1] * mat[4] * mat[11] - mat[0] * mat[5] * mat[11];
	temp[12] = mat[6] * mat[9] * mat[12] - mat[5] * mat[10] * mat[12] - mat[6] * mat[8] * mat[13] + mat[4] * mat[10] * mat[13] + mat[5] * mat[8] * mat[14] - mat[4] * mat[9] * mat[14];
	temp[13] = mat[1] * mat[10] * mat[12] - mat[2] * mat[9] * mat[12] + mat[2] * mat[8] * mat[13] - mat[0] * mat[10] * mat[13] - mat[1] * mat[8] * mat[14] + mat[0] * mat[9] * mat[14];
	temp[14] = mat[2] * mat[5] * mat[12] - mat[1] * mat[6] * mat[12] - mat[2] * mat[4] * mat[13] + mat[0] * mat[6] * mat[13] + mat[1] * mat[4] * mat[14] - mat[0] * mat[5] * mat[14];
	temp[15] = mat[1] * mat[6] * mat[8] - mat[2] * mat[5] * mat[8] + mat[2] * mat[4] * mat[9] - mat[0] * mat[6] * mat[9] - mat[1] * mat[4] * mat[10] + mat[0] * mat[5] * mat[10];


	/// check determinant if it is 0
	det = mat[3] * mat[6] * mat[9] * mat[12] - mat[2] * mat[7] * mat[9] * mat[12] - mat[3] * mat[5] * mat[10] * mat[12] + mat[1] * mat[7] * mat[10] * mat[12] +
		mat[2] * mat[5] * mat[11] * mat[12] - mat[1] * mat[6] * mat[11] * mat[12] - mat[3] * mat[6] * mat[8] * mat[13] + mat[2] * mat[7] * mat[8] * mat[13] +
		mat[3] * mat[4] * mat[10] * mat[13] - mat[0] * mat[7] * mat[10] * mat[13] - mat[2] * mat[4] * mat[11] * mat[13] + mat[0] * mat[6] * mat[11] * mat[13] +
		mat[3] * mat[5] * mat[8] * mat[14] - mat[1] * mat[7] * mat[8] * mat[14] - mat[3] * mat[4] * mat[9] * mat[14] + mat[0] * mat[7] * mat[9] * mat[14] +
		mat[1] * mat[4] * mat[11] * mat[14] - mat[0] * mat[5] * mat[11] * mat[14] - mat[2] * mat[5] * mat[8] * mat[15] + mat[1] * mat[6] * mat[8] * mat[15] +
		mat[2] * mat[4] * mat[9] * mat[15] - mat[0] * mat[6] * mat[9] * mat[15] - mat[1] * mat[4] * mat[10] * mat[15] + mat[0] * mat[5] * mat[10] * mat[15];
	if (fabs(det) <= 0.00001f) ///Epsilon
	{
		return identity(); /// cannot inverse, make it identity matrix
	}

	/// Divide with determinatn
	invDet = 1.0f / det;

	return Matrix4(invDet * temp[0], invDet * temp[1], invDet * temp[2], invDet * temp[3],
		invDet * temp[4], invDet * temp[5], invDet * temp[6], invDet * temp[7],
		invDet * temp[8], invDet * temp[9], invDet * temp[10], invDet * temp[11],
		invDet * temp[12], invDet * temp[13], invDet * temp[14], invDet * temp[15]);
}

inline Matrix4 Matrix4::rotX(const float& angle){

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

	return Matrix4( 1, 0, 0,
					0, c, -s,
					0, s, c );
}

inline Matrix4 Matrix4::rotY(const float& angle){

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

	return Matrix4( c, 0, s,
					0, 1, 0,
					-s, 0, c);
}

inline Matrix4 Matrix4::rotZ(const float& angle){

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

	return Matrix4( c, -s, 0,
					s, c, 0,
					0, 0, 1 );
}

///Rotate around vector.
inline Matrix4 Matrix4::rotVector(const float& angle, const Vector4& vec){

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

	return Matrix4( ///TODO: Make less cluttered.
		(x*x) + ((1 - (x*x))*c), (x*y*(1 - c)) - (x*s), (x*z*(1 - c)) + (y*s),
		(x*z*(1 - c)) + (z*s), (y*y*(1 - (y*y))*c), (x*z*(1 - c)) - (x*s),
		(x*z*(1 - c)) - (y*s), (y*z*(1 - c)) + (x*s), (z*z) + ((1 - (z*z))*c)
		);
}

inline Matrix4 Matrix4::translation(const float& x, const float& y, const float& z){

	return Matrix4( 1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1);
}

inline Matrix4 Matrix4::translation(const Vector4& vec){

	return Matrix4( 1, 0, 0, vec[0],
			0, 1, 0, vec[1],
			0, 0, 1, vec[2],
			0, 0, 0, 1);
}

inline Matrix4 Matrix4::scale(const float& x, const float& y, const float& z)
{
	return Matrix4( x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
}

inline Matrix4 Matrix4::scale(const Vector4& v)
{
	return Matrix4( v[0],  0,     0,     0,
			0,     v[1],  0,     0,
			0,     0,     v[2],  0,
			0,     0,     0,     1);
}

inline Vector4 Matrix4::getPosition()
{
  return Vector4(m[3], m[7], m[11], m[15]);
}



inline Matrix4 Matrix4::perspectiveMatrix(const float& farZ, const float& nearZ, const float& aspect, const float& fov)
{
	float verticalFOV = (fov*PI / 180.0f);
	float f = 1 / tan(verticalFOV / 2);
	float zRange = (nearZ - farZ);
	return Matrix4( f / aspect, 0,					0,									0,
					0,			f,					0,									0,
					0,			0,					((farZ + nearZ) / zRange),	((2 * farZ * nearZ) / zRange),
					0,			0,					-1,									0
				  );
}

inline Matrix4 Matrix4::viewMatrix(const float& pitch, const float& yaw, const Vector3& camerapos)
{
	double cosPitch = cos(pitch);
	double sinPitch = sin(pitch);
	double cosYaw = cos(yaw);
	double sinYaw = sin(yaw);

	Vector3 xaxis = { (float)cosYaw, 0.0f, (float)-sinYaw };
	Vector3 yaxis = { (float)(sinYaw * sinPitch), (float)cosPitch, (float)(cosYaw * sinPitch) };
	Vector3 zaxis = { (float)(sinYaw * cosPitch), (float)-sinPitch, (float)(cosPitch * cosYaw) };

	Matrix4 viewMatrix(
		xaxis[0], xaxis[1], xaxis[2], -xaxis.dot(camerapos),
		yaxis[0], yaxis[1], yaxis[2], -yaxis.dot(camerapos),
		zaxis[0], zaxis[1], zaxis[2], -zaxis.dot(camerapos),
		0.0f, 0.0f, 0.0f, 1.0f);

	return viewMatrix;
}

///  Friend Functions
inline Matrix4 operator*(float scalar, const Matrix4& rhs)
{
	return Matrix4(scalar*rhs[0], scalar*rhs[1], scalar*rhs[2], scalar*rhs[3], scalar*rhs[4], scalar*rhs[5], scalar*rhs[6], scalar*rhs[7], scalar*rhs[8], scalar*rhs[9], scalar*rhs[10], scalar*rhs[11], scalar*rhs[12], scalar*rhs[13], scalar*rhs[14], scalar*rhs[15]);
}

inline Vector4 operator*(const Vector4& vec, const Matrix4& mat)
{
	return Vector4(vec[0] * mat[0] + vec[1] * mat[1] + vec[2] * mat[2] + vec[3] * mat[3], vec[0] * mat[4] + vec[1] * mat[5] + vec[2] * mat[6] + vec[3] * mat[7], vec[0] * mat[8] + vec[1] * mat[9] + vec[2] * mat[10] + vec[3] * mat[11], vec[0] * mat[12] + vec[1] * mat[13] + vec[2] * mat[14] + vec[3] * mat[15]);
}

}

//#pragma pack(pop)