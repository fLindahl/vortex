#pragma once
#include "vector3.h"
#include <cmath>

#pragma pack(push, 1)

namespace Math
{

class Vector4 
{
public:
	Vector4();
	Vector4(const Vector3&, float);
	Vector4(float, float, float, float);
	~Vector4();

	float operator[](int i) const;

	float& operator[](int i);
	
	const float* get();		///Get array

	bool operator==(const Vector4& rhs) const;
	Vector4 operator+(const Vector4& rhs) const;
	Vector4 operator-(const Vector4& rhs) const;
	Vector4 operator/(const Vector4& rhs) const;
	Vector4 operator+(const float& scalar) const;
	Vector4 operator-(const float& scalar) const;
	Vector4 operator*(const float& scalar) const;
	Vector4 operator/(const float& scalar) const;
	Vector4 cross(const Vector4& rhs) const;
	float dot(const Vector4& rhs) const;
	float length() const;
	float lengthsq() const;
	Vector4 normalized() const;

	float x() { return vec[0]; };
	float y() { return vec[1]; };
	float z() { return vec[2]; };
	float w() { return vec[3]; };
	
	Vector3 xyz() { return Vector3(vec[0], vec[1], vec[2]); };
	
	static Vector4 lerp(const Vector4& va, const Vector4& vb, const float& t);

private:
	float vec[4];
};

inline Vector4::Vector4()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
	vec[3] = 1.0f;
}

inline Vector4::Vector4(const Vector3& v, float w = 1.0f)
{
	vec[0] = v[0], vec[1] = v[1], vec[2] = v[2], vec[3] = w;
}

inline Vector4::Vector4(float a, float b, float c, float d = 1.0f)
{
	vec[0] = a, vec[1] = b, vec[2] = c, vec[3] = d;
}

inline Vector4::~Vector4()
{

}

inline const float* Vector4::get()
{
	return vec;
}

inline float Vector4::operator[](int i) const
{
	return vec[i];
}

inline float& Vector4::operator[](int i)
{
	return vec[i];
}

inline bool Vector4::operator==(const Vector4& rhs) const
{
	return (vec[0] == rhs[0] && vec[1] == rhs[1] && vec[2] == rhs[2]);
}

inline Vector4 Vector4::operator+(const Vector4& rhs) const
{
	return Vector4(vec[0] + rhs[0], vec[1] + rhs[1], vec[2] + rhs[2]);
}

inline Vector4 Vector4::operator-(const Vector4& rhs) const
{
	return Vector4(vec[0] - rhs[0], vec[1] - rhs[1], vec[2] - rhs[2]);
}

inline Vector4 Vector4::operator/(const Vector4& rhs) const
{
	return Vector4(vec[0] / rhs[0], vec[1] / rhs[1], vec[2] / rhs[2]);
}

inline Vector4 Vector4::operator+(const float& scalar) const
{
	return Vector4(vec[0] + scalar, vec[1] + scalar, vec[2] + scalar);
}

inline Vector4 Vector4::operator-(const float& scalar) const
{
	return Vector4(vec[0] - scalar, vec[1] - scalar, vec[2] - scalar);
}

inline Vector4 Vector4::operator*(const float& scalar) const
{
	return Vector4(vec[0] * scalar, vec[1] * scalar, vec[2] * scalar);
}

inline Vector4 Vector4::operator/(const float& scalar) const
{
	return Vector4(vec[0] / scalar, vec[1] / scalar, vec[2] / scalar);
}

inline Vector4 Vector4::cross(const Vector4& rhs) const
{
	return Vector4(vec[1]*rhs[2] - vec[2]*rhs[1], vec[2]*rhs[0] - vec[0]*rhs[2], vec[0]*rhs[1] - vec[1]*rhs[0]);
}

inline float Vector4::dot(const Vector4& rhs) const
{
	return (vec[0]*rhs[0] + vec[1]*rhs[1] + vec[2]*rhs[2]);
}

inline float Vector4::length() const
{
	return float(sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]));
}

inline float Vector4::lengthsq() const
{
	return float(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

inline Vector4 Vector4::normalized() const
{
	float l = this->length();
	return Vector4(vec[0] / l, vec[1] / l, vec[2] / l);
}

inline Vector4 Vector4::lerp(const Vector4& va, const Vector4& vb, const float& t) 
{
  return (va + ((vb-va) * t)); 
}

}

#pragma pack(pop)