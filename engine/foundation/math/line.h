#pragma once

#include <cfloat>
#include "vector4.h"
#include "plane.h"

namespace Math
{
#define EPSILON 0.0000001

class Line
{
public:
    Line();
    ~Line();

    bool Intersect(Vector4& out, const Plane& plane);

//private:
    Vector4 p;
    Vector4 m;
};

}