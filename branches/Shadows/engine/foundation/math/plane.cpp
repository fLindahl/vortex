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

	plane::plane(const vec4& p, const vec4& n)
	{
		float d = vec4::dot3(p, n);
		this->vec = n;
		this->vec.set_w(-d);
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

	bool plane::intersectline(const vec4& startPoint, const vec4& endPoint, vec4& outIntersectPoint) const
	{

		float v1 = vec4::dot3(this->vec, startPoint);
		float v2 = vec4::dot3(this->vec, endPoint);
		float d = (v1 - v2);
		if (abs(d) < 0.0000001f)
		{
			return false;
		}

		d = 1.0f / d;

		float pd = vec4::dot(this->vec, startPoint.vec);
		pd *= d;

		vec4 p = (endPoint - startPoint) * pd;

		p += startPoint;
		outIntersectPoint = p;

		return true;
	}

	plane plane::normalize(const plane& p)
	{
		vec4 f(p.vec);
		float len = f.length3();
		if (len < 0.0000001f)
		{
			return p;
		}
		f *= 1.0f / len;
		plane ret;
		ret.vec = f;
		return ret;
	}
}
