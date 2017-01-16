#pragma once
#include <cfloat>
#include "vector4.h"
#include "plane.h"
#include "bbox.h"

namespace Math
{

class line
{
public:
    line();
    line(const vec4& startPos, const vec4& dirAndMagnitude);
    line(const vec4& startPos, const vec4& direction, const float& magnitude);
    ~line();


	void set(const vec4& startPos, const vec4& dirAndMagnitude);

	const vector& vec() const;

	const point& start() const;
	point end() const;

    void transform(const Math::mat4& t);

	/// intersect with line
	bool IntersectLine(const line& l, point& pa, point& pb) const;

    /// Returns true if the line intersects a plane. NOTE: out.w will be 1.0f/magnitude multiplied with the distance to the plane (range 0.0f to 1.0f)
    bool Intersect(vec4& out, const Math::plane& plane) const;

    // Returns true if the line intersects a bounding box. Make sure the bbox is aligned with worldspace axes.
    bool IntersectAABB(const Math::bbox& box) const;

	/// get length
	float length() const;
	/// get squared length
	float lengthsq() const;
	/// minimal distance of point to line
	float distance(const point& p) const;
	/// calculates shortest distance between lines
	float distance(const line& l, point& pa, point& pb) const;
	/// return t of the closest point on the line
	float closestpoint(const point& p) const;
	/// return p = b + m*t
	point pointat(float t) const;
	


//private:
    vec4 p;
    vec4 m;
};

}