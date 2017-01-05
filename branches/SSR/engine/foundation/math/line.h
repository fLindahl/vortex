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

    void transform(const Math::mat4& t);

    /// Returns true if the line intersects a plane. NOTE: out.w will be 1.0f/magnitude multiplied with the distance to the plane (range 0.0f to 1.0f)
    bool Intersect(vec4& out, const Math::plane& plane);

    // Returns true if the line intersects a bounding box. Make sure the bbox is aligned with worldspace axes.
    bool IntersectAABB(const Math::bbox& box);

//private:
    vec4 p;
    vec4 m;
};

}