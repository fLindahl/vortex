#include "config.h"
#include "line.h"
#include "plane.h"

namespace Math
{

Line::Line()
{

}

Line::Line(const vec4& startPos, const vec4& dirAndMagnitude)
{
    this->p = startPos;
    this->m = dirAndMagnitude;
}

Line::Line(const vec4& startPos, const vec4& direction, const float& magnitude)
{
    this->p = startPos;
    this->m = direction * magnitude;
}

Line::~Line()
{

}

bool Line::Intersect(vec4& out, const plane& plane)
{
    vec4 ab = (this->p + this->m) - this->p;

    float denom1 = vec4::dot(plane.n(), this->p);
    float denom2 = vec4::dot(plane.n(), ab);
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