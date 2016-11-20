#include "config.h"
#include "line.h"
#include "matrix4.h"
#include "math.h"

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
        out.w() = t;
        return true;
    }

    return false;
}

void line::transform(const Math::mat4 &t)
{
    this->p = Math::mat4::transform(this->p, t);
    this->m = Math::mat4::transform(this->m, t);
}
bool line::IntersectAABB(const Math::bbox &box)
{
    double invMX = ((this->m.x() == 0.0f) ? 0.0f : 1.0/this->m.x());
    double invMY = ((this->m.y() == 0.0f) ? 0.0f : 1.0/this->m.y());
    double invMZ = ((this->m.z() == 0.0f) ? 0.0f : 1.0/this->m.z());

    double tx1 = (box.minPoint.x() - this->p.x())*invMX;
    double tx2 = (box.maxPoint.x() - this->p.x())*invMX;

    double tmin = Math::min(tx1, tx2);
    double tmax = Math::max(tx1, tx2);

    double ty1 = (box.minPoint.y() - this->p.y())*invMY;
    double ty2 = (box.maxPoint.y() - this->p.y())*invMY;

    tmin = Math::max(tmin, Math::min(ty1, ty2));
    tmax = Math::min(tmax, Math::max(ty1, ty2));

    double tz1 = (box.minPoint.z() - this->p.z())*invMZ;
    double tz2 = (box.maxPoint.z() - this->p.z())*invMZ;

    tmin = Math::max(tmin, Math::min(tz1, tz2));
    tmax = Math::min(tmax, Math::max(tz1, tz2));

    return tmax >= tmin;
}

}