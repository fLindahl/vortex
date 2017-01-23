#include "config.h"
#include "line.h"
#include "matrix4.h"
#include "math.h"

namespace Math
{

line::line()
{

}

line::line(const vec4& startPos, const vector& dirAndMagnitude)
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

const vector& line::vec() const
{
	return this->m;
}

point line::start() const
{
	return this->p;
}

point line::end() const
{
	return this->p + this->m;
}

void line::set(const point& startpoint, const point& endpoint)
{
	this->p = startpoint;
	this->m = endpoint - startpoint;
}

//Get line/line intersection. Returns the shortest line between two lines.
//Borrowed from nebula
bool line::IntersectLine(const line& l, point& pa, point& pb) const
{
	const float EPS = 2.22e-16f;
	point p1 = this->p;
	point p2 = this->pointat(10.0f);
	point p3 = l.p;
	point p4 = l.pointat(10.0f);

	vector p13 = p1 - p3;
	vector p43 = p4 - p3;
	vector p21 = p2 - p1;
	if (p43.lengthsq() < EPS) return false;
	if (p21.lengthsq() < EPS) return false;

	float d1343 = vec4::dot3(p13, p43);
	float d4321 = vec4::dot3(p43, p21);
	float d1321 = vec4::dot3(p13, p21);
	float d4343 = vec4::dot3(p43, p43);
	float d2121 = vec4::dot3(p21, p21);

	float denom = d2121 * d4343 - d4321 * d4321;
	if (abs(denom) < EPS) return false;
	float numer = d1343 * d4321 - d1321 * d4343;

	float mua = numer / denom;
	float mub = (d1343 + d4321 * (mua)) / d4343;

	pa = p1 + p21 * mua;
	pb = p3 + p43 * mub;

	return true;
}

bool line::Intersect(vec4& out, const Math::plane& plane) const
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
bool line::IntersectAABB(const Math::bbox &box) const 
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

float line::length() const
{
	return this->m.length();
}

float line::lengthsq() const
{
	return this->m.lengthsq();
}

float line::closestpoint(const point& p) const
{
	vector diff(p - this->p);
	float l = vec4::dot3(this->m, this->m);
	if (l > 0.0f)
	{
		float t = vec4::dot3(this->m, diff) / l;
		return t;
	}
	else
	{
		return 0.0f;
	}
}

float line::distance(const point& p) const
{
	vector diff(p - this->p);
	float l = vec4::dot3(this->m, this->m);
	if (l > 0.0f)
	{
		float t = vec4::dot3(this->m, diff) / l;
		diff = diff - this->m * t;
		return diff.length();
	}
	else
	{
		// line is really a point...
		vector v(p - this->p);
		return v.length();
	}
}

float line::distance(const line& l, point& pa, point& pb) const
{
	const float EPS = 2.22e-16f;
	vector u = this->pointat(10) - this->p;
	vector v = l.pointat(10) - l.p;
	vector w = this->p - l.p;

	float a = vec4::dot3(u, u);
	float b = vec4::dot3(u, v);
	float c = vec4::dot3(v, v);
	float d = vec4::dot3(u, w);
	float e = vec4::dot3(v, w);
	float D = a*c - b * b;
	float sc, tc;

	if (D < EPS)
	{
		sc = 0.0f;
		tc = (b > c ? d / b : e / c);
	}
	else
	{
		sc = (b * e - c * d) / D;
		tc = (a * e - b * d) / D;
	}

	// calculate points
	pa = this->pointat(sc);
	pb = l.pointat(tc);

	vector dp = w + (u * sc) - (v * tc);
	return dp.length3();
}

point line::pointat(float t) const
{
	return this->p + this->m * t;
}

}