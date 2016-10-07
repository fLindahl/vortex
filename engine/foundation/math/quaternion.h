#pragma once
//------------------------------------------------------------------------------
/**
    @class Quaternion
*/

#include "vector4.h"
#include "matrix4.h"
//------------------------------------------------------------------------------

namespace Math
{

class Quaternion
{
public:
    /// default constructor, NOTE: does NOT setup components!
    Quaternion();
    /// construct from components
    Quaternion(float x, float y, float z, float w);
    /// construct from Vector4
    Quaternion(Vector4);
    
    
    /// assignment operator
    void operator=(const Quaternion& rhs);
    /// equality operator
    bool operator==(const Quaternion& rhs) const;
    /// inequality operator
    bool operator!=(const Quaternion& rhs) const;
    
    Quaternion operator*(const float& scalar) const;
    Quaternion operator/(const float& scalar) const;
    Quaternion operator+(const Quaternion& rhs) const;
    Quaternion operator-(const Quaternion& rhs) const;

    /// set content
    void set(float x, float y, float z, float w);
    /// set from Vector4
    void set(const Vector4 f4);
    /// set the x component
    void set_x(float x);
    /// set the y component
    void set_y(float y);
    /// set the z component
    void set_z(float z);
    /// set the w component
    void set_w(float w);

    /// read/write access to x component
    float& x();
    /// read/write access to y component
    float& y();
    /// read/write access to z component
    float& z();
    /// read/write access to w component
    float& w();
    /// read-only access to x component
    float x() const;
    /// read-only access to y component
    float y() const;
    /// read-only access to z component
    float z() const;
    /// read-only access to w component
    float w() const;

    /// return true if Quaternion is identity
    bool isidentity() const;
    /// returns length
    float length() const;
    
    /// return Quaternion in barycentric coordinates
    static Quaternion barycentric(const Quaternion& q0, const Quaternion& q1, const Quaternion& q2, float f, float g);
    /// return dot product of two normalized quaternions
    static float dot(const Quaternion& q0, const Quaternion& q1);
    /// conjugates and renormalizes quaternion
    static Quaternion inverse(const Quaternion& q);
    /// multiply 2 quaternions
    static Quaternion multiply(const Quaternion& q0, const Quaternion& q1);
    /// compute unit length Quaternion
    static Quaternion normalize(const Quaternion& q);
    /// build quaternion from axis and clockwise rotation angle in radians
    static Quaternion rotationaxis(const Vector4& axis, float angle);
    /// build quaternion from rotation matrix
    static Quaternion rotationmatrix(const Matrix4& m);
    /// build quaternion from yaw, pitch and roll
    static Quaternion rotationyawpitchroll(float yaw, float pitch, float roll);
    
    /// interpolate between 2 quaternions using linear interpolation
    static Quaternion lerp(const Quaternion &q1, const Quaternion &q2, float t);
    
    /// interpolate between 2 quaternion using spherical interpolation
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
    
private:
    friend class Matrix4;
    Vector4 vec;
    
};

inline Quaternion::Quaternion()
{
    vec = Vector4();
}

inline Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->vec = Vector4(x, y, z, w);
}

inline Quaternion::Quaternion(Vector4 v)
{
    this->vec = v;
}

inline void Quaternion::operator=(const Quaternion& rhs)
{
    this->vec = rhs.vec;
}

inline bool Quaternion::operator==(const Quaternion& rhs) const
{
	return rhs.vec == this->vec;
}

inline bool Quaternion::operator!=(const Quaternion& rhs) const
{
    return !(this->operator==(rhs));
}

inline Quaternion Quaternion::operator*(const float& scalar) const
{
  return Quaternion(this->vec * scalar);
}

inline Quaternion Quaternion::operator/(const float& scalar) const
{
  return Quaternion(this->vec / scalar);
}

inline Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
  return Quaternion(this->vec - rhs.vec);
}

inline Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
  return Quaternion(this->vec + rhs.vec);
}

inline void Quaternion::set(float x, float y, float z, float w)
{
    this->vec = Vector4(x,y,z,w);
}

inline void Quaternion::set_x(float x)
{
    this->vec[0] = x;
}

inline void Quaternion::set_y(float y)
{
    this->vec[1] = y;
}

inline void Quaternion::set_z(float z)
{
    this->vec[2] = z;
}

inline void Quaternion::set_w(float w)
{
    this->vec[3] = w;
}

inline void Quaternion::set(const Vector4 f4)
{
    this->vec = f4;
}

inline float& Quaternion::x()
{
  return vec[0];
}

inline float& Quaternion::y()
{
    return vec[1];
}

inline float& Quaternion::z()
{
    return vec[2];
}

inline float& Quaternion::w()
{
    return vec[3];
}

inline float Quaternion::x() const
{
    return vec[0];
}

inline float Quaternion::y() const
{
    return vec[1];
}

inline float Quaternion::z() const
{
    return vec[2];
}

inline float Quaternion::w() const
{
    return vec[3];
}

inline bool Quaternion::isidentity() const
{
	const Quaternion id(0,0,0,1);
    return id == *this;
}

inline float Quaternion::length() const
{
    return this->vec.length();
}

inline Quaternion Quaternion::barycentric(const Quaternion& q0, const Quaternion& q1, const Quaternion& q2, float f, float g)
{
	float s = f + g;
	if(s != 0.0f)
	{
		Quaternion a = Quaternion::slerp(q0,q1,s);
		Quaternion b = Quaternion::slerp(q0,q2,s);
		Quaternion res = Quaternion::slerp(a,b,g/s);
		return res;
	}
	else
	{
		return q0;
	}
}

inline float Quaternion::dot(const Quaternion& q0, const Quaternion& q1)
{
	return q0.vec.dot(q1.vec);
}

inline Quaternion Quaternion::inverse(const Quaternion& q)
{
	//scalar len = q.lengthsq();
	//if(len > 0.00001f)
	//{
	//	quaternion con = quaternion::conjugate(q);
	//	con.vec *= 1.0f / len;
	//	return con;
	//}
	return Quaternion(0.0f,0.0f,0.0f,0.0f);
}

inline Quaternion Quaternion::multiply(const Quaternion& q0, const Quaternion& q1)
{
  // NOT IMPLEMENTED!! CHECK ONLINE FOR HOW TO DO IT
    return q1;
}

inline Quaternion Quaternion::normalize(const Quaternion& q)
{
    return Quaternion(q.vec.normalized());
}

inline Quaternion Quaternion::rotationaxis(const Vector4& axis, float angle)
{
    return Quaternion(0,0,0,0);//Vectormath::Aos::Quat::rotation(angle, Vectormath::Aos::Vector3(axis.vec));
}

inline Quaternion Quaternion::rotationmatrix(const Matrix4& m)
{
	// FIXME write own implementation

	float trace = m[0] + m[5] + m[10];
	Matrix4 mm = m.transpose();
	float temp[4];

	if (trace > 0.0f)
	{
		float s = sqrt(trace + 1.0f);
		temp[3]=(s * 0.5f);
		s = 0.5f / s;

		temp[0]=((mm[9] - mm[6]) * s);
		temp[1]=((mm[2] - mm[8]) * s);
		temp[2]=((mm[4] - mm[1]) * s);
	} 
	else 
	{
		int i = mm[0] < mm[5] ? 
			(mm[5] < mm[10] ? 2 : 1) :
			(mm[0] < mm[10] ? 2 : 0); 
		int j = (i + 1) % 3;  
		int k = (i + 2) % 3;

		float s = sqrt(mm[i + i * 4] - mm[j + j * 4] - mm[k + k * 4] + 1.0f);
		temp[i] = s * 0.5f;
		s = 0.5f / s;

		//Find correct row / column
		// y + x * 4 
		
		temp[3] = (mm[j + k * 4] - mm[k + j * 4]) * s;
		temp[j] = (mm[i + j * 4] + mm[j + i * 4]) * s;
		temp[k] = (mm[i + k * 4] + mm[k + i * 4]) * s;
	}
	Quaternion q(temp[0],temp[1],temp[2],temp[3]);
	return q;
}

inline Quaternion Quaternion::rotationyawpitchroll(float yaw, float pitch, float roll)
{

    float halfYaw = 0.5f * yaw;
    float halfPitch = 0.5f * pitch;
    float halfRoll = 0.5f * roll;
    float cosYaw = cos(halfYaw);
    float sinYaw = sin(halfYaw);
    float cosPitch = cos(halfPitch);
    float sinPitch = sin(halfPitch);
    float cosRoll = cos(halfRoll);
    float sinRoll = sin(halfRoll);
    Quaternion q(-(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw),
		-(cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw),
		-(sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw),
		-(cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw));

    return q;
}

inline Quaternion Quaternion::lerp(const Quaternion &q1, const Quaternion &q2, float t) 
{
  return Quaternion::normalize(q1*(1-t) + q2*t);
}

inline Quaternion Quaternion::slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion q = Quaternion();
	
  	double cosHalfTheta = q0.x()*q1.x()+q0.y()*q1.y()+q0.z()*q1.z()+q0.w()*q1.w();
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(cosHalfTheta) == 1.0)
	{
		return q0;
	}
	bool reverse_q1 = false;
	if (cosHalfTheta < 0) // Always follow the shortest path
	{
		reverse_q1 = true;
		cosHalfTheta = -cosHalfTheta;
	}
	// Calculate temporary values.
	const double halfTheta = acos(cosHalfTheta);
	const double sinHalfTheta = std::sqrt(1.0 - (cosHalfTheta * cosHalfTheta));
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (std::abs(sinHalfTheta) < 0.001)
	{
		if (!reverse_q1)
			 for (int i=0;i < 4;i++) q.vec[i] = (1-t)*q0.vec[i] + t*q1.vec[i];
		else for (int i=0;i < 4;i++) q.vec[i] = (1-t)*q0.vec[i] - t*q1.vec[i];
		return q;
	}
	const double A = sin((1-t) * halfTheta)/sinHalfTheta;
	const double B = sin(t*halfTheta)/sinHalfTheta;
	if (!reverse_q1)
		 for (int i=0;i < 4;i++) q.vec[i] = A*q0.vec[i] + B*q1.vec[i];
	else for (int i=0;i < 4;i++) q.vec[i] = A*q0.vec[i] - B*q1.vec[i];
	
	return q;
}

}