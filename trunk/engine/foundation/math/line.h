#pragma once
#include <cfloat>
#include "vector4.h"
#include "plane.h"

namespace Math
{

class Line
{
public:
    Line();
    Line(const vec4& startPos, const vec4& dirAndMagnitude);
    Line(const vec4& startPos, const vec4& direction, const float& magnitude);
    ~Line();

    bool Intersect(vec4& out, const Math::plane& plane);

//private:
    vec4 p;
    vec4 m;
};

}