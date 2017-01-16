#pragma once
#include "vector4.h"

namespace Math
{

class plane
{
public:
	///Empty plane
    plane();
    ///Construct plane from 3 points
    plane(const vec4& a, const vec4& b, const vec4& c);
	///Construct from point and normal
	plane(const vec4& p, const vec4& n);

    ~plane();

    void constructFromPoints(const vec4& a, const vec4& b, const vec4& c);

    float d() const;
    vec4 n() const;
    vec4 p() const;

	bool intersectline(const vec4& startPoint, const vec4& endPoint, vec4& outIntersectPoint) const;
	
	static plane normalize(const plane& p);

private:
	friend class mat4;
    vec4 vec;
};

}