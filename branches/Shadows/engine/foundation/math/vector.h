#pragma once
#include "vector4.h"

//------------------------------------------------------------------------------
namespace Math
{
class vector;

#if (defined(_XM_VMX128_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_))
typedef const vector  __VectorArg;
#else
// win32 VC++ does not support passing aligned objects as value so far
// here is a bug-report at microsoft saying so:
// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=334581
typedef const vector& __VectorArg;
#endif

class VORTEX_ALIGN16 vector : public vec4
{
public:
    /// default constructor
    vector();
    /// construct from components
    vector(float x, float y, float z);
	/// construct from single component
	vector(float v);
    /// construct from vec4
    vector(const vec4& rhs);
    /// construct from vmVector4
    vector(const __m128 &rhs);
    /// return the null vector
    static vector nullvec();
    /// return the standard up vector (0, 1, 0)
    static vector upvec();
    /// assignment operator
    void operator=(const vector& rhs);
    /// assign vmVector4
    void operator=(const __m128 &rhs);
    /// flip sign
    vector operator-() const;
    /// add vector inplace
    void operator+=(const vector& rhs);
    /// subtract vector inplace
    void operator-=(const vector& rhs);
    /// scale vector inplace
    void operator*=(float s);
    /// add 2 vectors
    vector operator+(const vector& rhs) const;
    /// subtract 2 vectors
    vector operator-(const vector& rhs) const;
    /// scale vector
    vector operator*(float s) const;
    /// equality operator
    bool operator==(const vector& rhs) const;
    /// inequality operator
    bool operator!=(const vector& rhs) const;
    /// set components
    void set(float x, float y, float z);

    friend class point;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector() :
    vec4(0.0f, 0.0f, 0.0f, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector(float x, float y, float z) :
    vec4(x, y, z, 0.0f)
{
    // empty
}
	
//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector(float v) :
	vec4(v, v, v, 0.0f)
{
		// empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector(const vec4& rhs) :
    vec4(rhs)
{
    this->set_w(0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector(const __m128 &rhs) :
    vec4(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::nullvec()
{
    return vector(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::upvec()
{
    return vector(0.0f, 1.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator=(const vector& rhs)
{
    this->vec = rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator=(const __m128 &rhs)
{
    this->vec.vec = rhs;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator-() const
{
    static const  mm128_ivec negmask = {static_cast<int>(0x80000000),static_cast<int>(0x80000000),static_cast<int>(0x80000000),static_cast<int>(0x80000000)};
    return  _mm_xor_ps(negmask,this->vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator+=(const vector& rhs)
{
    vec4::operator+=(rhs);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator-=(const vector& rhs)
{
    vec4::operator-=(rhs);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator*=(float s)
{
    vec4::operator*=(s);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator+(const vector& rhs) const
{
    return vec4::operator+(rhs);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator-(const vector& rhs) const
{
    return vec4::operator-(rhs);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator*(float s) const
{
    return vec4::operator*(s);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
vector::operator==(const vector& rhs) const
{
    return vec4::operator==(rhs.vec);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
vector::operator!=(const vector& rhs) const
{
    return vec4::operator!=(rhs.vec);
}    

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::set(float x, float y, float z)
{
    vec4::set(x, y, z, 0.0f);
}

} // namespace Math
//------------------------------------------------------------------------------
