//------------------------------------------------------------------------
//
//  Name:   math.h
//
//  Author: Fredrik Lindahl
//
//  Desc:   Math Library
//
//------------------------------------------------------------------------


#pragma once
#include <stdlib.h>
#include "vector4.h"

namespace Math
{
	//returns a random float between zero and 1 
	inline float randFloat()
	{ 
		return ((rand()) / (RAND_MAX + 1.0f)); 
	}

	//returns a random float in the range -1 < n < 1 
	inline float randomClamped()    { return randFloat() - randFloat(); }

	template <class T>
	// Min. function. Returns the smallest value of a and b
	// Is a less than b? if so, return a, else return b
	static const T& min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	
	template <class T>
	// Max. function. Returns the largest value of a and b
	// Is a less than b? if so, return b, else return a
	static const T& max(const T& a, const T& b)
	{
		return (a < b) ? b : a;
	}

	
	template <class T>
	// Clamps a value between to other values. 
	// Returns a max or min if in-value is greater or less than interval range
	static const T& clamp(const T& in, const T& min, const T& max)
	{
		return (in < min) ? min : ((in > max) ? max : in);
	}

	static void barycentric(const vec4& p, const vec4& a, const vec4& b, const vec4& c, float& u, float& v, float& w)
	{
		vec4 v0 = b - a;
		vec4 v1 = c - a;
		vec4 v2 = p - a;

		float d00 = Math::vec4::dot(v0,v0);
		float d01 = Math::vec4::dot(v0,v1);
		float d11 = Math::vec4::dot(v1,v1);
		float d20 = Math::vec4::dot(v2,v0);
		float d21 = Math::vec4::dot(v2,v1);

		float denom = 1 / (d00 * d11 - d01 * d01);

		v = (d11 * d20 - d01 * d21) * denom;
		w = (d00 * d21 - d01 * d20) * denom;

		u = 1.0f - v - w;
	}
}