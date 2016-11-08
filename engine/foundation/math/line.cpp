#include "config.h"
#include "line.h"
#include "plane.h"

namespace Math
{

Line::Line()
{

}

Line::Line(const Vector4& startPos, const Vector4& dirAndMagnitude)
{
    this->p = startPos;
    this->m = dirAndMagnitude;
}

Line::Line(const Vector4& startPos, const Vector4& direction, const float& magnitude)
{
    this->p = startPos;
    this->m = direction * magnitude;
}

Line::~Line()
{

}

bool Line::Intersect(Vector4& out, const Plane& plane)
{
    Vector4 ab = (this->p + this->m) - this->p;

    float denom1 = Vector4::dot(plane.n(), this->p);
    float denom2 = Vector4::dot(plane.n(), ab);
    float d = plane.d();
    float t = (d - denom1 / denom2);

    if (t >= 0.0f && t <= 1.0f)
    {
        out = this->p + (ab * t);
        return true;
    }

    return false;
}

}