#include "config.h"
#include "plane.h"

namespace Math
{

	plane::plane(const vec4& a, const vec4& b, const vec4& c)
    {
        this->vec = vec4::normalize(vec4::cross3(b - a, c - a));
        this->vec.w() = vec4::dot(vec, a);
    }

	plane::~plane()
    {

    }

	float plane::d() const
    {
        return this->vec.w();
    }

	vec4 plane::n() const
    {
        return this->vec;//vec4(this->vec.x(), this->vec.y(), this->vec.z(), 1);
    }

	vec4 plane::p() const
    {
        vec4 ret = vec4(this->vec * this->vec.w());
        ret.w() = 1.0f;
        return ret;
    }
}
