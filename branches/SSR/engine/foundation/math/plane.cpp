#include "config.h"
#include "plane.h"

namespace Math
{
    plane::plane() :
        vec(0.0f)
    {

    }

	plane::plane(const vec4& a, const vec4& b, const vec4& c)
    {
        this->constructFromPoints(a,b,c);
    }

	plane::~plane()
    {

    }

    void plane::constructFromPoints(const vec4 &a, const vec4 &b, const vec4 &c)
    {
        this->vec = vec4::normalize(vec4::cross3(b - a, c - a));
        this->vec.w() = vec4::dot(vec, a);
    }

	float plane::d() const
    {
        return this->vec.w();
    }

	vec4 plane::n() const
    {
        return vec4(this->vec.x(), this->vec.y(), this->vec.z(), 0.0f);
    }

	vec4 plane::p() const
    {
        vec4 ret = vec4(this->vec * this->vec.w());
        ret.w() = 1.0f;
        return ret;
    }
}
