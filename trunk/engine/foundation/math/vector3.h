#pragma once
#include <cmath>
using namespace std;

namespace Math
{

class Vector3 
{
public: 
	Vector3();
	Vector3(float*);
	Vector3(float,float,float);
	~Vector3();

	float operator[](int i) const;

	bool operator==(const Vector3& rhs) const;
	bool operator!=(const Vector3& rhs) const;
	const float* get();
	void set(const float& x, const float& y, const float& z);
	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator-(const Vector3& rhs) const;
	Vector3 operator/(const Vector3& rhs) const;
	Vector3 operator+(const float& scalar) const;
	Vector3 operator-(const float& scalar) const;
	Vector3 operator*(const float& scalar) const;
	Vector3 operator/(const float& scalar) const;
	Vector3 cross(const Vector3& rhs) const;
	float dot(const Vector3& rhs) const;
	float length() const;
	Vector3 normalized() const;

private:
	float vec[3];
};

inline Vector3::Vector3()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
}

inline Vector3::Vector3(float* arr)
{
	vec[0] = arr[0];
	vec[1] = arr[1];
	vec[2] = arr[2];
}

inline Vector3::Vector3(float a, float b, float c)
{
	vec[0] = a;
	vec[1] = b;
	vec[2] = c;
}

inline Vector3::~Vector3()
{

}

inline float Vector3::operator[](int i) const
{
	return vec[i];
}

inline bool Vector3::operator==(const Vector3& rhs) const
{
	return (vec[0] == rhs[0] && vec[1] == rhs[1] && vec[2] == rhs[2]);
}

inline bool Vector3::operator!=(const Vector3& rhs) const
{
	return !(vec[0] == rhs[0] && vec[1] == rhs[1] && vec[2] == rhs[2]);
}

inline const float* Vector3::get()
{
	return vec;
}

inline void Vector3::set(const float& x, const float& y, const float& z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

inline Vector3 Vector3::operator+(const Vector3& rhs) const
{
	return Vector3(vec[0] + rhs[0], vec[1] + rhs[1], vec[2] + rhs[2]);
}

inline Vector3 Vector3::operator-(const Vector3& rhs) const
{
	return Vector3(vec[0] - rhs[0], vec[1] - rhs[1], vec[2] - rhs[2]);
}

inline Vector3 Vector3::operator/(const Vector3& rhs) const
{
	return Vector3(vec[0] / rhs[0], vec[1] / rhs[1], vec[2] / rhs[2]);
}

inline Vector3 Vector3::operator+(const float& scalar) const
{
	return Vector3(vec[0] + scalar, vec[1] + scalar, vec[2] + scalar);
}

inline Vector3 Vector3::operator-(const float& scalar) const
{
	return Vector3(vec[0] - scalar, vec[1] - scalar, vec[2] - scalar);
}

inline Vector3 Vector3::operator*(const float& scalar) const
{
	return Vector3(vec[0] * scalar, vec[1] * scalar, vec[2] * scalar);
}

inline Vector3 Vector3::operator/(const float& scalar) const
{
	return Vector3(vec[0] / scalar, vec[1] / scalar, vec[2] / scalar);
}

inline Vector3 Vector3::cross(const Vector3& rhs) const
{
	return Vector3(vec[0] * rhs[2] - vec[2] * rhs[1], vec[2] * rhs[0] - vec[0] * rhs[2], vec[0] * rhs[1] - vec[1] * rhs[0]);
}

inline float Vector3::dot(const Vector3& rhs) const
{
	return (vec[0] * rhs[0] + vec[1] * rhs[1] + vec[2] * rhs[2]);
}

inline float Vector3::length() const
{
	return float(sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
}

inline Vector3 Vector3::normalized() const
{
	float length = this->length();
	return Vector3(vec[0] / length, vec[1] / length, vec[2] / length);
}

}