#include "config.h"
#include "line.h"
#include "matrix4.h"

namespace Math
{

line::line()
{

}

line::line(const vec4& startPos, const vec4& dirAndMagnitude)
{
    this->p = startPos;
    this->m = dirAndMagnitude;
}

line::line(const vec4& startPos, const vec4& direction, const float& magnitude)
{
    this->p = startPos;
    this->m = direction * magnitude;
}

line::~line()
{

}

bool line::Intersect(vec4& out, const Math::plane& plane)
{
    vec4 endPoint = (this->p + this->m);

    vec4 ab = endPoint - this->p;

    float pd = vec4::dot3(plane.n(), this->p);
    float denom = vec4::dot3(plane.n(), ab);

    if(denom > 0.0f)
    {
        return false;
    }

    float t = (plane.d() - pd) / denom;

    if(t >= 0.0f && t <= 1.0f)
    {
        out = this->p + (ab * t);
        return true;
    }

    return false;
}

void line::transform(const Math::mat4 &t)
{
    this->p = Math::mat4::transform(this->p, t);
    this->m = Math::mat4::transform(this->m, t);
}

}