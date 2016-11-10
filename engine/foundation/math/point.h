#pragma once
#include "vector4.h"

//------------------------------------------------------------------------------
namespace Math
{
class point;

#if (defined(_XM_VMX128_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_))
typedef const point  __PointArg;
#else
// win32 VC++ does not support passing aligned objects as value so far
// here is a bug-report at microsoft saying so:
// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=334581
typedef const point& __PointArg;
#endif


class VORTEX_ALIGN16 point : public vec4
{
public:
    /// default constructor
    point();
    /// construct from components
    point(float x, float y, float z);
    /// construct from vec4
    point(const vec4& rhs);
    /// !!!! copy constructor forbidden, otherwise passing point's to a function
    /// !!!! via Registers doesnt work
    //point(const point& rhs);
    /// construct from vmVector4
    point(const __m128 &rhs);
    /// return a point at the origin (0, 0, 0)
    static point origin();
    /// assignment operator
    void operator=(const point& rhs);
    /// assign vmVector4
    void operator=(const __m128 &rhs);   
    /// set components
    void set(float x, float y, float z);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point() :
    vec4(0.0f, 0.0f, 0.0f, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(float x, float y, float z) :
    vec4(x, y, z, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(const vec4& rhs) :
    vec4(rhs)
{
    this->set_w(1.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(const __m128 & rhs) :
    vec4(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::origin()
{
    return point(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator=(const point& rhs)
{
    this->vec = rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::set(float x, float y, float z)
{
    vec4::set(x, y, z, 1.0f);
}

} // namespace Math
//------------------------------------------------------------------------------
