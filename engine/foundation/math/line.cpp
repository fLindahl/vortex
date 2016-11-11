#include "config.h"
#include "line.h"

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

bool Line::Intersect(vec4& out, const Math::plane& plane)
{
    vec4 endPoint = (this->p + this->m);

    vec4 ab = endPoint - this->p;

    float pd = vec4::dot3(plane.n(), this->p);
    float denom = vec4::dot3(plane.n(), ab);

    float t = (plane.d() - pd) / denom;

    if(t >= 0.0f && t <= 1.0f)
    {
        out = this->p + (ab * t);
        return true;
    }

    return false;
}

}