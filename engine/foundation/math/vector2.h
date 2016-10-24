#pragma once
#include <cmath>
using namespace std;

namespace Math
{


class Vector2 {
public: 
	Vector2();
	Vector2(const float*);
	Vector2(const float&, const float&);
	~Vector2();

	float operator[](const int& i) const;

	float x(){ return vec[0]; }
	float y(){ return vec[1]; }

	float x() const { return vec[0]; }
	float y() const { return vec[1]; }

	bool operator==(const Vector2& rhs) const;
	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	Vector2 operator/(const Vector2& rhs) const;
	Vector2 operator+(const float& scalar) const;
	Vector2 operator-(const float& scalar) const;
	Vector2 operator*(const float& scalar) const;
	Vector2 operator/(const float& scalar) const;
	float dot(const Vector2& rhs) const;
	float length() const;
	Vector2 normalized() const;

private:
	float vec[2];
};

inline Vector2::Vector2(){
	vec[0] = vec[1] = 0.0f;
}

inline Vector2::Vector2(const float* arr){
	vec[0] = arr[0];
	vec[1] = arr[1];
}

inline Vector2::Vector2(const float& a, const float& b){
	vec[0] = a, vec[1] = b;
}

inline Vector2::~Vector2(){

}

inline float Vector2::operator[](const int& i) const
{
	return vec[i];
}

inline bool Vector2::operator==(const Vector2& rhs) const
{
	return (vec[0] == rhs[0] && vec[1] == rhs[1]);
}

inline Vector2 Vector2::operator+(const Vector2& rhs) const
{
	return Vector2(vec[0] + rhs[0], vec[1] + rhs[1]);
}

inline Vector2 Vector2::operator-(const Vector2& rhs) const
{
	return Vector2(vec[0] - rhs[0], vec[1] - rhs[1]);
}

inline Vector2 Vector2::operator/(const Vector2& rhs) const
{
	return Vector2(vec[0] / rhs[0], vec[1] / rhs[1]);
}

inline Vector2 Vector2::operator+(const float& scalar) const
{
	return Vector2(vec[0] + scalar, vec[1] + scalar);
}

inline Vector2 Vector2::operator-(const float& scalar) const
{
	return Vector2(vec[0] - scalar, vec[1] - scalar);
}

inline Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(vec[0] * scalar, vec[1] * scalar);
}

inline Vector2 Vector2::operator/(const float& scalar) const
{
	return Vector2(vec[0] / scalar, vec[1] / scalar);
}

inline float Vector2::dot(const Vector2& rhs) const
{
	return (vec[0] * rhs[0] + vec[1] * rhs[1]);
}

inline float Vector2::length() const
{
	return float(sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
}

inline Vector2 Vector2::normalized() const
{
	float length = this->length();
	return Vector2(vec[0] / length, vec[1] / length);
}

}