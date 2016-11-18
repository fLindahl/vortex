#pragma once
#include <cfloat>
#include "vector4.h"
#include "plane.h"
#include "bbox.h"

namespace Math
{

class line
{
public:
    line();
    line(const vec4& startPos, const vec4& dirAndMagnitude);
    line(const vec4& startPos, const vec4& direction, const float& magnitude);
    ~line();

    void transform(const Math::mat4& t);

    bool Intersect(vec4& out, const Math::plane& plane);

    bool IntersectAABB(const Math::bbox& box);

//private:
    vec4 p;
    vec4 m;
};

}