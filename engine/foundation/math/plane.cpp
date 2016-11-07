#include "config.h"
#include "plane.h"

namespace Math
{

    Plane::Plane(const Vector4& a, const Vector4& b, const Vector4& c)
    {
        this->vec = Vector4::normalize(Vector4::cross(b - a, c - a));
        this->vec[3] = 1.0f;
        this->vec[3] = Vector4::dot(vec, a);
    }

    Plane::~Plane()
    {

    }

    const float& Plane::d() const
    {
        return this->vec.w();
    }

    Vector4 Plane::n() const
    {
        return Vector4(this->vec[0], this->vec[1], this->vec[2], 1);
    }

    Vector4 Plane::p() const
    {
        Vector4 ret = Vector4(this->vec * this->vec[3]);
        ret.w() = 1.0f;
        return ret;
    }
}
