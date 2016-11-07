#include "config.h"
#include "line.h"

namespace Math
{

Line::Line()
{

}

Line::~Line()
{

}

bool Line::Intersect(Vector4& out, const Plane &plane)
{
    Vector4 ab = (this->p + this->m) - this->p;
    float t = (plane.d() - Vector4::dot(plane.n(), this->p) / Vector4::dot(plane.n(), ab));

    if (t >= EPSILON && t <= 1.0f)
    {
        out = this->p + (ab * t);
        return true;
    }

    return false;
}

}