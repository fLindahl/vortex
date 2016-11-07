#pragma once
#include "vector4.h"

namespace Math
{

class plane
{
public:
    ///Construct plane from 3 points
    plane();
    plane(const vec4& a, const vec4& b, const vec4& c);
    ~plane();

    void constructFromPoints(const vec4& a, const vec4& b, const vec4& c);

    float d() const;
    vec4 n() const;
    vec4 p() const;

private:
	friend class mat4;
    vec4 vec;
};

}