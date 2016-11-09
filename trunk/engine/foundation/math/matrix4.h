#pragma once
#include <cmath>
#include "vector4.h"
#include "matrix3.h"
#include "quaternion.h"

#define PI  3.14159265358979323846f

//#pragma pack(push, 1)

namespace Math
{
	/// Constructor start
	class Matrix4{
	public:
		///Initiate identity matrix
		Matrix4();

		///construct from float array
		Matrix4(const float f[16]);

		///initiate 4D Matrix with all inputs.
		Matrix4(const float& m0, const float& m1, const float& m2, const float& m3,
			const float& m4, const float& m5, const float& m6, const float& m7,
			const float& m8, const float& m9, const float& m10, const float& m11,
			const float& m12, const float& m13, const float& m14, const float& m15);

		///Initiate homogenous 4d matrix.
		Matrix4(const float& m0, const float& m1, const float& m2,	///take only the 9 first variables since we only want to initiate those in an homogenous 4D Matrix.
			const float& m4, const float& m5, const float& m6,	///Notice the names too.
			const float& m8, const float& m9, const float& m10);

		~Matrix4();

		void set(const float& m00, const float& m01, const float& m02, const float& m03,
				 const float& m04, const float& m05, const float& m06, const float& m07,
				 const float& m08, const float& m09, const float& m10, const float& m11,
				 const float& m12, const float& m13, const float& m14, const float& m15);		///Set values
		const float* get();		///Get array
		Matrix3 get3x3();		///Get array

		float getDeterminant() const;
		float getCofactor(const float& m0, const float& m1, const float& m2, const float& m3, const float& m4, const float& m5, const float& m6, const float& m7, const float& m8) const;

		Vector4 operator*(const Vector4& rhs) const;	///product Mat*VectA = VectB
		Matrix4 operator*(const Matrix4& rhs) const;	///product MatA*MatB = MatC
		Matrix4 operator+(const Matrix4& rhs) const;	///addition MatA+MatB = MatC
		Matrix4 operator-(const Matrix4& rhs) const;

		bool operator==(const Matrix4& rhs) const;	///equality operator
		bool operator!=(const Matrix4& rhs) const;	///inequality operator

		float operator[](const int& i) const;

		static Matrix4 identity();	/// returns an identity matrix
		Matrix4 transpose() const;	/// Transpose Function
		Matrix4 invert() const;		/// Invert Function 
		static Matrix4 inverse(const Matrix4& mat);		/// Inverse

		Vector4 get_xaxis() const;
		Vector4 get_yaxis() const;
		Vector4 get_zaxis() const;
		Vector4 getPosition() const;

		void translate(const float& x, const float& y, const float& z);
		void translate(const Vector3& vec);
		void translate(const Vector4& vec);

		///Init rotational matrices
		static Matrix4 rotX(const float& angle);
		static Matrix4 rotY(const float& angle);
		static Matrix4 rotZ(const float& angle);
		static Matrix4 rotVector(const float& angle, const Vector4& vec);
		static Matrix4 translation(const float& x, const float& y, const float& z);
		static Matrix4 translation(const Vector4& vec);
		static Matrix4 scale(const float& x, const float& y, const float& z);
		static Matrix4 scale(const Vector4& v);

		static Matrix4 perspectiveMatrix(const float& farZ, const float& nearZ, const float& aspect, const float& fov);
		static Matrix4 viewMatrix(const float& pitch, const float& yaw, const Vector3& camerapos);
		friend Matrix4 operator*(const float& scalar, const Matrix4& m);		///Scalar product
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

	inline Matrix4::Matrix4(const float f[16])
	{
		this->m[0] = f[0];
		this->m[1] = f[1];
		this->m[2] = f[2];
		this->m[3] = f[3];
		this->m[4] = f[4];
		this->m[5] = f[5];
		this->m[6] = f[6];
		this->m[7] = f[7];
		this->m[8] = f[8];
		this->m[9] = f[9];
		this->m[10] = f[10];
		this->m[11] = f[11];
		this->m[12] = f[12];
		this->m[13] = f[13];
		this->m[14] = f[14];
		this->m[15] = f[15];
	}

	inline Matrix4::Matrix4(const float& m0, const float& m1, const float& m2, const float& m3,
							const float& m4, const float& m5, const float& m6, const float& m7,
							const float& m8, const float& m9, const float& m10, const float& m11,
							const float& m12, const float& m13, const float& m14, const float& m15)
	{
		set(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15); ///Set private array from arguments
	}
	inline Matrix4::Matrix4(const float& m00, const float& m01, const float& m02,
							const float& m04, const float& m05, const float& m06,
							const float& m08, const float& m09, const float& m10
							)
	{
		float m03 = 0;
		float m07 = 0;
		float m11 = 0;
		float m12 = 0;
		float m13 = 0;
		float m14 = 0;
		float m15 = 1;

		set(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15); ///Set private array from arguments
	}

	inline Matrix4::~Matrix4()
	{

	}

	inline void Matrix4::set(const float& m00, const float& m01, const float& m02, const float& m03,
							 const float& m04, const float& m05, const float& m06, const float& m07,
							 const float& m08, const float& m09, const float& m10, const float& m11,
							 const float& m12, const float& m13, const float& m14, const float& m15)
	{
		m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
		m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
		m[8] = m08;  m[9] = m09;  m[10]= m10;  m[11]= m11;
		m[12]= m12;  m[13]= m13;  m[14]= m14;  m[15]= m15;
	}

	inline const float* Matrix4::get()
	{
		return m;
	}

	inline Matrix3 Matrix4::get3x3()
	{
		return Matrix3(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);
	}

	inline Vector4 Matrix4::operator*(const Vector4& rhs) const{
		return Vector4(m[0]*rhs[0] + m[4]*rhs[1] + m[8]*rhs[2] + m[12]*rhs[3],
					   m[1]*rhs[0] + m[5]*rhs[1] + m[9]*rhs[2] + m[13]*rhs[3],
					   m[2]*rhs[0] + m[6]*rhs[1] + m[10]*rhs[2] + m[14]*rhs[3],
					   m[3]*rhs[0] + m[7]*rhs[1] + m[11]*rhs[2] + m[15]*rhs[3]);
	}

/*
	inline Matrix4 Matrix4::operator*(const Matrix4& rhs) const
	{
		float a00=this->m[0];
		float a01=this->m[1];
		float a02=this->m[2];
		float a03=this->m[3];

		float a10=this->m[4];
		float a11=this->m[5];
		float a12=this->m[6];
		float a13=this->m[7];

		float a20=this->m[8];
		float a21=this->m[9];
		float a22=this->m[10];
		float a23=this->m[11];

		float a30=this->m[12];
		float a31=this->m[13];
		float a32=this->m[14];
		float a33=this->m[15];

		float b00=rhs.m[0];
		float b01=rhs.m[1];
		float b02=rhs.m[2];
		float b03=rhs.m[3];

		float b10=rhs.m[4];
		float b11=rhs.m[5];
		float b12=rhs.m[6];
		float b13=rhs.m[7];

		float b20=rhs.m[8];
		float b21=rhs.m[9];
		float b22=rhs.m[10];
		float b23=rhs.m[11];

		float b30=rhs.m[12];
		float b31=rhs.m[13];
		float b32=rhs.m[14];
		float b33=rhs.m[15];

		Math::Matrix4 matrix;
		matrix.m[0] = a00*b00+a10*b01+a20*b02+a30*b03;
		matrix.m[1] = a01*b00+a11*b01+a21*b02+a31*b03;
		matrix.m[2] = a02*b00+a12*b01+a22*b02+a32*b03;
		matrix.m[3] = a03*b00+a13*b01+a23*b02+a33*b03;

		matrix.m[4] = a00*b10+a10*b11+a20*b12+a30*b13;
		matrix.m[5] = a01*b10+a11*b11+a21*b12+a31*b13;
		matrix.m[6] = a02*b10+a12*b11+a22*b12+a32*b13;
		matrix.m[7] = a03*b10+a13*b11+a23*b12+a33*b13;

		matrix.m[8] = a00*b20+a10*b21+a20*b22+a30*b23;
		matrix.m[9] = a01*b20+a11*b21+a21*b22+a31*b23;
		matrix.m[10] = a02*b20+a12*b21+a22*b22+a32*b23;
		matrix.m[11] = a03*b20+a13*b21+a23*b22+a33*b23;

		matrix.m[12] = a00*b30+a10*b31+a20*b32+a30*b33;
		matrix.m[13] = a01*b30+a11*b31+a21*b32+a31*b33;
		matrix.m[14] = a02*b30+a12*b31+a22*b32+a32*b33;
		matrix.m[15] = a03*b30+a13*b31+a23*b32+a33*b33;

		return matrix;
	}
*/
	inline Matrix4 Matrix4::operator*(const Matrix4& n) const
	{
		return Matrix4(m[0]*n[0]  + m[4]*n[1]  + m[8]*n[2]  + m[12]*n[3],   m[1]*n[0]  + m[5]*n[1]  + m[9]*n[2]  + m[13]*n[3],   m[2]*n[0]  + m[6]*n[1]  + m[10]*n[2]  + m[14]*n[3],   m[3]*n[0]  + m[7]*n[1]  + m[11]*n[2]  + m[15]*n[3],
					   m[0]*n[4]  + m[4]*n[5]  + m[8]*n[6]  + m[12]*n[7],   m[1]*n[4]  + m[5]*n[5]  + m[9]*n[6]  + m[13]*n[7],   m[2]*n[4]  + m[6]*n[5]  + m[10]*n[6]  + m[14]*n[7],   m[3]*n[4]  + m[7]*n[5]  + m[11]*n[6]  + m[15]*n[7],
					   m[0]*n[8]  + m[4]*n[9]  + m[8]*n[10] + m[12]*n[11],  m[1]*n[8]  + m[5]*n[9]  + m[9]*n[10] + m[13]*n[11],  m[2]*n[8]  + m[6]*n[9]  + m[10]*n[10] + m[14]*n[11],  m[3]*n[8]  + m[7]*n[9]  + m[11]*n[10] + m[15]*n[11],
					   m[0]*n[12] + m[4]*n[13] + m[8]*n[14] + m[12]*n[15],  m[1]*n[12] + m[5]*n[13] + m[9]*n[14] + m[13]*n[15],  m[2]*n[12] + m[6]*n[13] + m[10]*n[14] + m[14]*n[15],  m[3]*n[12] + m[7]*n[13] + m[11]*n[14] + m[15]*n[15]);
	}

	inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const{
		return Matrix4(m[0]+rhs[0],   m[1]+rhs[1],   m[2]+rhs[2],   m[3]+rhs[3],
					   m[4]+rhs[4],   m[5]+rhs[5],   m[6]+rhs[6],   m[7]+rhs[7],
					   m[8]+rhs[8],   m[9]+rhs[9],   m[10]+rhs[10], m[11]+rhs[11],
					   m[12]+rhs[12], m[13]+rhs[13], m[14]+rhs[14], m[15]+rhs[15]);
	}

	inline Matrix4 Matrix4::operator-(const Matrix4 &rhs) const
	{
		return Matrix4(m[0]-rhs[0],   m[1]-rhs[1],   m[2]-rhs[2],   m[3]-rhs[3],
					   m[4]-rhs[4],   m[5]-rhs[5],   m[6]-rhs[6],   m[7]-rhs[7],
					   m[8]-rhs[8],   m[9]-rhs[9],   m[10]-rhs[10], m[11]-rhs[11],
					   m[12]-rhs[12], m[13]-rhs[13], m[14]-rhs[14], m[15]-rhs[15]);
	}

	inline bool Matrix4::operator==(const Matrix4& rhs) const{
		return (m[0] == rhs[0] && m[1] == rhs[1] && m[2] == rhs[2] && m[3] == rhs[3] &&
			m[4] == rhs[4] && m[5] == rhs[5] && m[6] == rhs[6] && m[7] == rhs[7] &&
			m[8] == rhs[8] && m[9] == rhs[9] && m[10] == rhs[10] && m[11] == rhs[11] &&
			m[12] == rhs[12] && m[13] == rhs[13] && m[14] == rhs[14] && m[15] == rhs[15]);
	}

	inline bool Matrix4::operator!=(const Matrix4& rhs) const{
		return (m[0] != rhs[0] || m[1] != rhs[1] || m[2] != rhs[2] || m[3] != rhs[3] ||
			m[4] != rhs[4] || m[5] != rhs[5] || m[6] != rhs[6] || m[7] != rhs[7] ||
			m[8] != rhs[8] || m[9] != rhs[9] || m[10] != rhs[10] || m[11] != rhs[11] ||
			m[12] != rhs[12] || m[13] != rhs[13] || m[14] != rhs[14] || m[15] != rhs[15]);
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

inline Matrix4 Matrix4::invert() const
{
	// get cofactors of minor matrices
	float cofactor0 = getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
	float cofactor1 = getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
	float cofactor2 = getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
	float cofactor3 = getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);

	// get determinant
	float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
	if(fabs(determinant) <= 0.00000001f)
	{
		return identity();
	}

	// get rest of cofactors for adj(M)
	float cofactor4 = getCofactor(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
	float cofactor5 = getCofactor(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
	float cofactor6 = getCofactor(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
	float cofactor7 = getCofactor(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);

	float cofactor8 = getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[13],m[14],m[15]);
	float cofactor9 = getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[12],m[14],m[15]);
	float cofactor10= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[12],m[13],m[15]);
	float cofactor11= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[12],m[13],m[14]);

	float cofactor12= getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[9], m[10],m[11]);
	float cofactor13= getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[8], m[10],m[11]);
	float cofactor14= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[8], m[9], m[11]);
	float cofactor15= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[8], m[9], m[10]);

	Matrix4 temp = Matrix4::identity();

	// build inverse matrix = adj(M) / det(M)
	// adjugate of M is the transpose of the cofactor matrix of M
	float invDeterminant = 1.0f / determinant;
	temp.m[0] =  invDeterminant * cofactor0;
	temp.m[1] = -invDeterminant * cofactor4;
	temp.m[2] =  invDeterminant * cofactor8;
	temp.m[3] = -invDeterminant * cofactor12;

	temp.m[4] = -invDeterminant * cofactor1;
	temp.m[5] =  invDeterminant * cofactor5;
	temp.m[6] = -invDeterminant * cofactor9;
	temp.m[7] =  invDeterminant * cofactor13;

	temp.m[8] =  invDeterminant * cofactor2;
	temp.m[9] = -invDeterminant * cofactor6;
	temp.m[10]=  invDeterminant * cofactor10;
	temp.m[11]= -invDeterminant * cofactor14;

	temp.m[12]= -invDeterminant * cofactor3;
	temp.m[13]=  invDeterminant * cofactor7;
	temp.m[14]= -invDeterminant * cofactor11;
	temp.m[15]=  invDeterminant * cofactor15;

	return *this;
}

inline float Matrix4::getDeterminant() const
{
	return m[0] * getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]) -
		m[1] * getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]) +
		m[2] * getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]) -
		m[3] * getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);
}

inline float Matrix4::getCofactor(const float& m0, const float& m1, const float& m2,
						   const float& m3, const float& m4, const float& m5,
						   const float& m6, const float& m7, const float& m8) const
{
	return m0 * (m4 * m8 - m5 * m7) -
		m1 * (m3 * m8 - m5 * m6) +
		m2 * (m3 * m7 - m4 * m6);
}


inline Matrix4 Matrix4::inverse(const Matrix4& mat){

	return mat.invert();
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
					0, c, s,
					0, -s, c );
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

	return Matrix4( c, 0, -s,
					0, 1, 0,
					s, 0, c);
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

	return Matrix4( c, s, 0,
					-s, c, 0,
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

	return Matrix4( 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					x, y, z, 1);
}

inline Matrix4 Matrix4::translation(const Vector4& vec){

	return Matrix4( 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					vec[0], vec[1], vec[2], 1);
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

inline Vector4 Matrix4::get_xaxis() const
{
	return Vector4(m[0], m[1], m[2], m[3]);
}

inline Vector4 Matrix4::get_yaxis() const
{
	return Vector4(m[4], m[5], m[6], m[7]);
}

inline Vector4 Matrix4::get_zaxis() const
{
	return Vector4(m[8], m[9], m[10], m[11]);
}

inline Vector4 Matrix4::getPosition() const
{
	return Vector4(m[12], m[13], m[14], m[15]);
}

inline Matrix4 Matrix4::perspectiveMatrix(const float& farZ, const float& nearZ, const float& aspect, const float& fov)
{
	float verticalFOV = (fov*PI / 180.0f);
	float f = 1 / tan(verticalFOV / 2);
	float zRange = (nearZ - farZ);
	return Matrix4( f / aspect, 0,					0,									0,
					0,			f,					0,									0,
					0,			0,					((farZ + nearZ) / zRange),	       -1,
					0,			0,					((2 * farZ * nearZ) / zRange),		0
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
inline Matrix4 operator*(const float& scalar, const Matrix4& rhs)
{
	return Matrix4(scalar*rhs[0], scalar*rhs[1], scalar*rhs[2], scalar*rhs[3], scalar*rhs[4], scalar*rhs[5], scalar*rhs[6], scalar*rhs[7], scalar*rhs[8], scalar*rhs[9], scalar*rhs[10], scalar*rhs[11], scalar*rhs[12], scalar*rhs[13], scalar*rhs[14], scalar*rhs[15]);
}

inline Vector4 operator*(const Vector4& vec, const Matrix4& mat)
{
	return Vector4(vec[0] * mat[0] + vec[1] * mat[1] + vec[2] * mat[2] + vec[3] * mat[3], vec[0] * mat[4] + vec[1] * mat[5] + vec[2] * mat[6] + vec[3] * mat[7], vec[0] * mat[8] + vec[1] * mat[9] + vec[2] * mat[10] + vec[3] * mat[11], vec[0] * mat[12] + vec[1] * mat[13] + vec[2] * mat[14] + vec[3] * mat[15]);
}

inline Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	float xx = q.x() * q.x();
	float xy = q.x() * q.y();
	float xz = q.x() * q.z();
	float xw = q.x() * q.w();

	float yy = q.y() * q.y();
	float yz = q.y() * q.z();
	float yw = q.y() * q.w();

	float zz = q.z() * q.z();
	float zw = q.z() * q.w();

	float m00 = 1 - 2 * (yy + zz);
	float m01 = 2 * (xy - zw);
	float m02 = 2 * (xz + yw);

	float m10 = 2 * (xy + zw);
	float m11 = 1 - 2 * (xx + zz);
	float m12 = 2 * (yz - xw);

	float m20 = 2 * (xz - yw);
	float m21 = 2 * (yz + xw);
	float m22 = 1 - 2 * (xx + yy);

	float m03 = 0;
	float m13 = 0;
	float m23 = 0;
	float m30 = 0;
	float m31 = 0;
	float m32 = 0;
	float m33 = 1;

	return Matrix4(m00, m10, m20, m30,
				   m01, m11, m21, m31,
				   m02, m12, m22, m32,
				   m03, m13, m23, m33);
}

inline void Matrix4::translate(const float &x, const float &y, const float &z)
{
	this->m[12] += x;
	this->m[13] += y;
	this->m[14] += z;
}

inline void Matrix4::translate(const Vector3& vec)
{
	this->m[12] += vec[0];
	this->m[13] += vec[1];
	this->m[14] += vec[2];
}

inline void Matrix4::translate(const Vector4& vec)
{
	this->m[12] += vec[0];
	this->m[13] += vec[1];
	this->m[14] += vec[2];
}

}

//#pragma pack(pop)