//------------------------------------------------------------------------
//
//  Name:   vector2.h
//
//  Author: Fredrik Lindahl and Mat Buckland
//
//  Desc:   Math Library
//
//------------------------------------------------------------------------


#pragma once
#include "matrix2.h"
#include <stdlib.h>

namespace Math
{

	//-------------------- LinesIntersection2D-------------------------
	//
	//	Given 2 lines in 2D space AB, CD this returns true if an 
	//	intersection occurs and sets dist to the distance the intersection
	//  occurs along AB. Also sets the 2d vector point to the point of
	//  intersection
	//----------------------------------------------------------------- 
	inline bool LineIntersection2D(Vector2   A,
		Vector2   B,
		Vector2   C,
		Vector2   D,
		float&     dist,
		Vector2&  point)
	{

		float rTop = (A.y() - C.y())*(D.x() - C.x()) - (A.x() - C.x())*(D.y() - C.y());
		float rBot = (B.x() - A.x())*(D.y() - C.y()) - (B.y() - A.y())*(D.x() - C.x());

		float sTop = (A.y() - C.y())*(B.x() - A.x()) - (A.x() - C.x())*(B.y() - A.y());
		float sBot = (B.x() - A.x())*(D.y() - C.y()) - (B.y() - A.y())*(D.x() - C.x());

		if ((rBot == 0) || (sBot == 0))
		{
			//lines are parallel
			return false;
		}

		float r = rTop / rBot;
		float s = sTop / sBot;

		if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
		{
			dist = Vector2Distance(A, B) * r;

			point = (B - A) * r + A;

			return true;
		}

		else
		{
			dist = 0;

			return false;
		}
	}

	//--------------------- PointToWorldSpace -------------------------------- 
	// 
	//  Transforms a point from the agent's local space into world space 
	//------------------------------------------------------------------------ 
	inline Vector2 PointToWorldSpace(const Vector2 &point, const Vector2 &dir, const Vector2 &dirNormal, const Vector2 &targetPos)
	{
		//make a copy of the point 
		Vector2 TransPoint = point;

		//create a transformation matrix 
		Matrix2 matTransform;

		

		//rotate 
		matTransform.Rotate(dir, dirNormal);


		//and translate 
		matTransform.Translate(targetPos.x(), targetPos.y());

		//now transform the vertices 
		matTransform.TransformVector2Ds(TransPoint);


		return TransPoint;
	}

	//-------------------------- Vec2DRotateAroundOrigin --------------------------
	//
	//  rotates a vector ang rads around the origin
	//-----------------------------------------------------------------------------
	inline void Vec2DRotateAroundOrigin(Vector2& v, double ang)
	{
		//create a transformation matrix
		Matrix2 mat;

		//rotate
		mat.Rotate(ang);

		//now transform the object's vertices
		mat.TransformVector2Ds(v);
	}


	//returns a random float between zero and 1 
	inline float RandFloat()
	{ 
		return ((rand()) / (RAND_MAX + 1.0f)); 
	}

	//returns a random float in the range -1 < n < 1 
	inline float RandomClamped()    { return RandFloat() - RandFloat(); }



	
	template <class T>
	// Min. function. Returns the smallest value of a and b
	// Is a less than b? if so, return a, else return b
	static const T& Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	
	template <class T>
	// Max. function. Returns the largest value of a and b
	// Is a less than b? if so, return b, else return a
	static const T& Max(const T& a, const T& b)
	{
		return (a < b) ? b : a;
	}

	
	template <class T>
	// Clamps a value between to other values. 
	// Returns a max or min if in-value is greater or less than interval range
	static const T& Clamp(const T& in, const T& min, const T& max)
	{
		return (in < min) ? min : ((in > max) ? max : in);
	}
}