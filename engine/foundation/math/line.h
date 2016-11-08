#pragma once
#include <cfloat>
#include "vector4.h"

namespace Math
{
#define EPSILON 0.0000001

class Plane;

class Line
{
public:
    Line();
    Line(const Vector4& startPos, const Vector4& dirAndMagnitude);
    Line(const Vector4& startPos, const Vector4& direction, const float& magnitude);
    ~Line();

    bool Intersect(Vector4& out, const Plane& plane);

//private:
    Vector4 p;
    Vector4 m;
};

}