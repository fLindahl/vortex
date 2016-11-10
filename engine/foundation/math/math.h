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
}