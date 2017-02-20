#pragma once
#include "config.h"

#ifdef __GNUC__
#include <cmath>
#endif

//------------------------------------------------------------------------------
namespace Math
{
	class mat4;
	class vec4;
	
	//#if (defined(_XM_VMX128_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_))
	//typedef const vec4 __Float4Arg;
	//#else
	// win32 VC++ does not support passing aligned objects as value so far
	// here is a bug-report at microsoft saying so:
	// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=334581
	typedef const vec4& __Float4Arg;
	//#endif

	typedef VORTEX_ALIGN16 struct mm128_vec {
		union {
			float f[4];
			unsigned int u[4];
			__m128 vec;
		};
		__forceinline operator __m128() const { return vec; }
	} mm128_vec;

	typedef VORTEX_ALIGN16 struct mm128_ivec {
		union {
			int u[4];
			mm128_vec vec;
		};
		__forceinline operator __m128() const { return vec.vec; }
	} mm128_ivec;

	typedef VORTEX_ALIGN16 struct mm128_uivec {
		union {
			unsigned int u[4];
			mm128_vec vec;
		};
		__forceinline operator __m128() const { return vec.vec; }
	} mm128_uivec;
	const mm128_vec _zero = { 0.0f, 0.0f, 0.0f, 0.0f };
	const mm128_vec _id_x = { 1.0f, 0.0f, 0.0f, 0.0f };
	const mm128_vec _id_y = { 0.0f, 1.0f, 0.0f, 0.0f };
	const mm128_vec _id_z = { 0.0f, 0.0f, 1.0f, 0.0f };
	const mm128_vec _id_w = { 0.0f, 0.0f, 0.0f, 1.0f };
	const mm128_vec _minus1 = { -1.0f, -1.0f, -1.0f, -1.0f };
	const mm128_vec _plus1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	const mm128_uivec _sign = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };

	class VORTEX_ALIGN16 vec4
	{
	public:
		/// default constructor, NOTE: does NOT setup components!
		vec4();
		/// construct from values
		vec4(float x, float y, float z, float w);
		/// construct from single value
		vec4(float v);
		/// copy constructor
		vec4(const vec4& rhs);
		/// construct from SSE 128 byte float array
		vec4(const __m128& rhs);
		/// construct from SSE 128 byte float vector
		vec4(const mm128_vec& rhs);

		/// assignment operator
		void operator=(const vec4 &rhs);
		/// assign an vmVector4
		void operator=(const __m128 &rhs);
		/// flip sign
		vec4 operator-() const;
		/// inplace add
		void operator+=(const vec4 &rhs);
		/// inplace sub
		void operator-=(const vec4 &rhs);
		/// inplace float multiply
		void operator*=(float s);
		/// muliply by a vector component-wise
		void operator*=(const vec4& rhs);
		/// divide by a vector component-wise
		void operator/=(const vec4& rhs);
		/// add 2 vectors
		vec4 operator+(const vec4 &rhs) const;
		/// subtract 2 vectors
		vec4 operator-(const vec4 &rhs) const;
		/// multiply with float
		vec4 operator*(float s) const;
		/// equality operator
		bool operator==(const vec4 &rhs) const;
		/// inequality operator
		bool operator!=(const vec4 &rhs) const;

		/// load content from 16-byte-aligned memory
		void load(const float* ptr);
		/// load content from unaligned memory
		void loadu(const float* ptr);
		/// write content to 16-byte-aligned memory through the write cache
		void store(float* ptr) const;
		/// write content to unaligned memory through the write cache
		void storeu(float* ptr) const;
		/// stream content to 16-byte-aligned memory circumventing the write-cache
		void stream(float* ptr) const;

		/// load 3 floats into x,y,z from unaligned memory
		void load_float3(const void* ptr, float w);
		/// load from UByte4N packed vector, move range to -1..+1
		//void load_ubyte4n_signed(const void* ptr, float w);
		/// load from Byte4N packed vector
		//void load_byte4n(const void* ptr, float w);
		/// set content
		void set(float x, float y, float z, float w);
		/// set the x component
		void set_x(float x);
		/// set the y component
		void set_y(float y);
		/// set the z component
		void set_z(float z);
		/// set the w component
		void set_w(float w);

		/// read/write access to x component
		float& x();
		/// read/write access to y component
		float& y();
		/// read/write access to z component
		float& z();
		/// read/write access to w component
		float& w();
		/// read-only access to x component
		float x() const;
		/// read-only access to y component
		float y() const;
		/// read-only access to z component
		float z() const;
		/// read-only access to w component
		float w() const;
		/// read-only access to indexed component
		float& operator[](const int index);

		/// return length of vector
		float length() const;
		/// return length of vectors XYZ components
		float length3() const;
		/// return squared length of vector
		float lengthsq() const;
		/// return squared length of vectors XYZ components
		float lengthsq3() const;
		/// return component-wise absolute
		vec4 abs() const;

		/// returns an arbitrary orthogonal vector to v
		static vec4 orthogonal(const vec4 &v);
		/// return 1.0 / vec
		static vec4 reciprocal(const vec4 &v);
		/// return 1.0 / vec, faster version using approximation
		static vec4 reciprocalapprox(const vec4 &v);
		/// component-wise multiplication
		static vec4 multiply(const vec4 &v0, const vec4 &v1);
		/// component-wise multiply and add
		static vec4 multiplyadd(const vec4 &v0, const vec4 &v1, const vec4 &v2);
		/// component-wise divide
		static vec4 divide(const vec4 &v0, const vec4 &v1);
		/// return 3-dimensional cross product
		static vec4 cross3(const vec4 &v0, const vec4 &v1);
		/// return 4d dot product
		static float dot(const vec4 &v0, const vec4 &v2);
		/// return 3d dot product of vectors
		static float dot3(const vec4 &v0, const vec4 &v1);
		/// return point in barycentric coordinates
		static vec4 barycentric(const vec4 &v0, const vec4 &v1, const vec4 &v2, float f, float g);
		/// perform Catmull-Rom interpolation
		static vec4 catmullrom(const vec4 &v0, const vec4 &v1, const vec4 &v2, const vec4 &v3, float s);
		/// perform Hermite spline interpolation
		static vec4 hermite(const vec4 &v1, const vec4 &t1, const vec4 &v2, const vec4 &t2, float s);
		/// perform linear interpolation between 2 4d vectors
		static vec4 lerp(const vec4 &v0, const vec4 &v1, float s);
		/// return 4d vector made up of largest components of 2 vectors
		static vec4 maximize(const vec4 &v0, const vec4 &v1);
		/// return 4d vector made up of smallest components of 2 vectors
		static vec4 minimize(const vec4 &v0, const vec4 &v1);
		/// return normalized version of 4d vector
		static vec4 normalize(const vec4 &v);
		/// return normalized version of 4d vector using faster approximations (introduces small error)
		static vec4 normalizeapprox(const vec4 &v);
		/// return normalized version of 4d vector, ignoring w
		static vec4 normalize3(const vec4 &v);
		/// return normalized version of 4d vector using faster approximations (introduces small error), ignoring w
		static vec4 normalizeapprox3(const vec4 &v);
		/// transform 4d vector by mat4
		static
			//__declspec(deprecated)
			vec4 transform(__Float4Arg v, const mat4 &m);
		/// reflect a vector v
		static vec4 reflect(const vec4 &normal, const vec4 &incident);
		/// clamp to min/max vector
		static vec4 clamp(const vec4 & Clamp, const vec4 & vMin, const vec4 & vMax);
		/// angle between two vectors
		static float angle(const vec4 & v0, const vec4 & v1);
		/// returns vector of boolean values where the values of v1 or v2 corresponds to control
		static vec4 select(const vec4& v0, const vec4& v1, const vec4& control);
		/// returns a zero vector
		static vec4 zerovector();
		/// returns an up vector
		static vec4 upvec();
		/// returns a vector that points along the x axis
		static vec4 rightvec();
		/// returns a vector that points along the z axis
		static vec4 forwardvec();

		/// return distance squared between two points
		static float distancesq(Math::vec4 a, Math::vec4 b);

		/// return true if any XYZ component is less-then
		static bool less3_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZ components are less-then
		static bool less3_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZ component is less-or-equal
		static bool lessequal3_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZ components are less-or-equal
		static bool lessequal3_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZ component is greater
		static bool greater3_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZ components are greater
		static bool greater3_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZ component is greater-or-equal
		static bool greaterequal3_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZ components are greater-or-equal
		static bool greaterequal3_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZ component is equal
		static bool equal3_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZ components are equal
		static bool equal3_all(const vec4 &v0, const vec4 &v1);
		/// perform near equal comparison with given epsilon (3 components)
		static bool nearequal3(const vec4 &v0, const vec4 &v1, const vec4 &epsilon);

		/// return true if any XYZW component is less-then
		static bool less4_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZW components are less-then
		static bool less4_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZW component is less-or-equal
		static bool lessequal4_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZW components are less-or-equal
		static bool lessequal4_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZW component is greater
		static bool greater4_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZW components are greater
		static bool greater4_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZW component is greater-or-equal
		static bool greaterequal4_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZW components are greater-or-equal
		static bool greaterequal4_all(const vec4 &v0, const vec4 &v1);
		/// return true if any XYZW component is equal
		static bool equal4_any(const vec4 &v0, const vec4 &v1);
		/// return true if all XYZW components are equal
		static bool equal4_all(const vec4 &v0, const vec4 &v1);
		/// perform near equal comparison with given epsilon (4 components)
		static bool nearequal4(const vec4 &v0, const vec4 &v1, const vec4 &epsilon);

		/// perform less of XYZ and returns vector with results
		static vec4 less(const vec4& v0, const vec4& v1);
		/// perform greater of XYZ and returns vector with results
		static vec4 greater(const vec4& v0, const vec4& v1);
		/// perform equal of XYZ and returns vector with results
		static vec4 equal(const vec4& v0, const vec4& v1);

		/// unpack the first element from a __m128
		static float unpack_x(__m128 v);
		/// unpack the second element from a __m128
		static float unpack_y(__m128 v);
		/// unpack the third element from a __m128
		static float unpack_z(__m128 v);
		/// unpack the fourth element from a __m128
		static float unpack_w(__m128 v);
		/// splat float into each component of a vector
		static vec4 splat(float s);
		/// return a vector with all elements set to element n of v. 0 <= element <= 3
		static vec4 splat(const vec4 &v, uint element);
		/// return a vector with all elements set to v.x
		static vec4 splat_x(const vec4 &v);
		/// return a vector with all elements set to v.y
		static vec4 splat_y(const vec4 &v);
		/// return a vector with all elements set to v.z
		static vec4 splat_z(const vec4 &v);
		/// return a vector with all elements set to v.w
		static vec4 splat_w(const vec4 &v);
		/// merge components of 2 vectors into 1
		static vec4 permute(const vec4& v0, const vec4& v1, unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3);
		/// floor
		static vec4 floor(const vec4 &v);
		/// ceil
		static vec4 ceiling(const vec4 &v);

		mm128_vec vec;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4::vec4()
	{
		//  empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4::vec4(float x, float y, float z, float w)
	{
		this->vec.vec = _mm_setr_ps(x, y, z, w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4::vec4(float v)
	{
		this->vec.vec = _mm_set1_ps(v);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4::vec4(const __m128 &rhs)
	{
		this->vec.vec = rhs;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4::vec4(const vec4 &rhs)
	{
		this->vec.vec = rhs.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4::vec4(const mm128_vec &rhs) :
		vec(rhs)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator=(const vec4 &rhs)
	{
		this->vec = rhs.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator=(const __m128 &rhs)
	{
		this->vec.vec = rhs;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::operator==(const vec4 &rhs) const
	{
		__m128 vTemp = _mm_cmpeq_ps(this->vec, rhs.vec);
		return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::operator!=(const vec4 &rhs) const
	{
		__m128 vTemp = _mm_cmpeq_ps(this->vec, rhs.vec);
		return ((_mm_movemask_ps(vTemp) == 0x0f) == 0);
	}

	//------------------------------------------------------------------------------
	/**
	Load 4 floats from 16-byte-aligned memory.
	*/
	__forceinline void
		vec4::load(const float* ptr)
	{
		this->vec.vec = _mm_load_ps(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	Load 4 floats from unaligned memory.
	*/
	__forceinline void
		vec4::loadu(const float* ptr)
	{
		this->vec.vec = _mm_loadu_ps(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	Store to 16-byte-aligned float pointer.
	*/
	__forceinline void
		vec4::store(float* ptr) const
	{
		_mm_store_ps(ptr, this->vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	Store to non-aligned float pointer.
	*/
	__forceinline void
		vec4::storeu(float* ptr) const
	{
		_mm_storeu_ps(ptr, this->vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::stream(float* ptr) const
	{
		this->store(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::load_float3(const void* ptr, float w)
	{
		// FIXME ...
		float * source = (float*)ptr;
		this->vec.vec = _mm_setr_ps(source[0], source[1], source[2], w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::operator-() const
	{
		return vec4(_mm_xor_ps(_sign, this->vec.vec));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::operator*(float t) const
	{
		__m128 temp = _mm_set1_ps(t);
		return _mm_mul_ps(this->vec.vec, temp);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator*=(const vec4& rhs)
	{
		this->vec.vec = _mm_mul_ps(this->vec.vec, rhs.vec);
	}


	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator/=(const vec4& rhs)
	{
		this->vec.vec = _mm_div_ps(this->vec.vec, rhs.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator+=(const vec4 &rhs)
	{
		this->vec.vec = _mm_add_ps(this->vec.vec, rhs.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator-=(const vec4 &rhs)
	{
		this->vec.vec = _mm_sub_ps(this->vec.vec, rhs.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::operator*=(float s)
	{
		__m128 temp = _mm_set1_ps(s);
		this->vec.vec = _mm_mul_ps(this->vec.vec, temp);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::operator+(const vec4 &rhs) const
	{
		return _mm_add_ps(this->vec.vec, rhs.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::operator-(const vec4 &rhs) const
	{
		return _mm_sub_ps(this->vec.vec, rhs.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::set(float x, float y, float z, float w)
	{
		this->vec.vec = _mm_setr_ps(x, y, z, w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		vec4::x()
	{
		return this->vec.f[0];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::x() const
	{
		float temp;
		_mm_store_ss(&temp, this->vec.vec);
		return temp;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		vec4::y()
	{
		return this->vec.f[1];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::y() const
	{
		float ret;
		__m128 temp = _mm_shuffle_ps(this->vec.vec, this->vec.vec, _MM_SHUFFLE(1, 1, 1, 1));
		_mm_store_ss(&ret, temp);
		return ret;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		vec4::z()
	{
		return this->vec.f[2];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::z() const
	{
		float ret;
		__m128 temp = _mm_shuffle_ps(this->vec.vec, this->vec.vec, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(&ret, temp);
		return ret;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		vec4::w()
	{
		return this->vec.f[3];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::w() const
	{
		float ret;
		__m128 temp = _mm_shuffle_ps(this->vec.vec, this->vec.vec, _MM_SHUFFLE(3, 3, 3, 3));
		_mm_store_ss(&ret, temp);
		return ret;
	}


	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		vec4::operator[](const int index)
	{
		_assert(index < 4, "Subscript out of range!\n");
		return this->vec.f[index];
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::set_x(float x)
	{
		__m128 temp = _mm_load_ss(&x);
		this->vec.vec = _mm_move_ss(this->vec.vec, temp);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::set_y(float y)
	{
		__m128 temp2 = _mm_load_ps1(&y);
		this->vec.vec = _mm_blend_ps(this->vec.vec, temp2, 2);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::set_z(float z)
	{
		__m128 temp2 = _mm_load_ps1(&z);
		this->vec.vec = _mm_blend_ps(this->vec.vec, temp2, 4);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		vec4::set_w(float w)
	{
		__m128 temp2 = _mm_load_ps1(&w);
		this->vec.vec = _mm_blend_ps(this->vec.vec, temp2, 8);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::length() const
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->vec.vec, this->vec.vec, 0xF1)));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::length3() const
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->vec.vec, this->vec.vec, 0x71)));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::lengthsq() const
	{
		return _mm_cvtss_f32(_mm_dp_ps(this->vec.vec, this->vec.vec, 0xF1));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::lengthsq3() const
	{
		return _mm_cvtss_f32(_mm_dp_ps(this->vec.vec, this->vec.vec, 0x71));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::orthogonal(const vec4 &v)
	{
		if (0.0 != v.x())
		{
			return Math::vec4((-v.y() - v.z()) / v.x(), 1.0f, 1.0f, 0.0f);
		}
		else if (0.0 != v.y())
		{
			return Math::vec4(1.0, (-v.x() - v.z()) / v.y(), 1.0f, 0.0f);
		}
		else if (0.0 != v.z())
		{
			return Math::vec4(1.0f, 1.0f, (-v.x() - v.y()) / v.z(), 0.0f);
		}
		else
		{
			return Math::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::reciprocal(const vec4 &v)
	{
		return _mm_div_ps(_plus1, v.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::reciprocalapprox(const vec4 &v)
	{
		return _mm_rcp_ps(v.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::multiply(const vec4 &v0, const vec4 &v1)
	{
		return _mm_mul_ps(v0.vec.vec, v1.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::multiplyadd(const vec4 &v0, const vec4 &v1, const vec4 &v2)
	{
		return _mm_add_ps(_mm_mul_ps(v0.vec.vec, v1.vec), v2.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::divide(const vec4 &v0, const vec4 &v1)
	{
		return _mm_div_ps(v0.vec.vec, v1.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::abs() const
	{
		unsigned int val = 0x7fffffff;
		__m128 temp = _mm_set1_ps(*(float*)&val);
		return _mm_and_ps(this->vec.vec, temp);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::cross3(const vec4 &v0, const vec4 &v1)
	{
		__m128 tmp0, tmp1, tmp2, tmp3, result;
		tmp0 = _mm_shuffle_ps(v0.vec.vec, v0.vec.vec, _MM_SHUFFLE(3, 0, 2, 1));
		tmp1 = _mm_shuffle_ps(v1.vec.vec, v1.vec.vec, _MM_SHUFFLE(3, 1, 0, 2));
		tmp2 = _mm_shuffle_ps(v0.vec.vec, v0.vec.vec, _MM_SHUFFLE(3, 1, 0, 2));
		tmp3 = _mm_shuffle_ps(v1.vec.vec, v1.vec.vec, _MM_SHUFFLE(3, 0, 2, 1));
		result = _mm_mul_ps(tmp0, tmp1);
		result = _mm_sub_ps(result, _mm_mul_ps(tmp2, tmp3));
		return result;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::dot(const vec4 &v0, const vec4 &v1)
	{
		return _mm_cvtss_f32(_mm_dp_ps(v0.vec.vec, v1.vec.vec, 0xF1));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::dot3(const vec4 &v0, const vec4 &v1)
	{
		return _mm_cvtss_f32(_mm_dp_ps(v0.vec.vec, v1.vec.vec, 0x71));
	}

	//------------------------------------------------------------------------------
	/**
	calculates Result = v0 + f * (v1 - v0) + g * (v2 - v0)
	*/
	__forceinline vec4
		vec4::barycentric(const vec4 &v0, const vec4 &v1, const vec4 &v2, float f, float g)
	{
		__m128 R1 = _mm_sub_ps(v1.vec, v0.vec);
		__m128 SF = _mm_set_ps1(f);
		__m128 R2 = _mm_sub_ps(v2.vec, v0.vec);
		__m128 SG = _mm_set_ps1(g);
		R1 = _mm_mul_ps(R1, SF);
		R2 = _mm_mul_ps(R2, SG);
		R1 = _mm_add_ps(R1, v0.vec);
		R1 = _mm_add_ps(R1, R2);
		return R1;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::catmullrom(const vec4 &v0, const vec4 &v1, const vec4 &v2, const vec4 &v3, float s)
	{
		float s2 = s * s;
		float s3 = s * s2;

		__m128 P0 = _mm_set_ps1((-s3 + 2.0f * s2 - s) * 0.5f);
		__m128 P1 = _mm_set_ps1((3.0f * s3 - 5.0f * s2 + 2.0f) * 0.5f);
		__m128 P2 = _mm_set_ps1((-3.0f * s3 + 4.0f * s2 + s) * 0.5f);
		__m128 P3 = _mm_set_ps1((s3 - s2) * 0.5f);

		P0 = _mm_mul_ps(P0, v0.vec);
		P1 = _mm_mul_ps(P1, v1.vec);
		P2 = _mm_mul_ps(P2, v2.vec);
		P3 = _mm_mul_ps(P3, v3.vec);
		P0 = _mm_add_ps(P0, P1);
		P2 = _mm_add_ps(P2, P3);
		P0 = _mm_add_ps(P0, P2);
		return P0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::hermite(const vec4 &v1, const vec4 &t1, const vec4 &v2, const vec4 &t2, float s)
	{
		float s2 = s * s;
		float s3 = s * s2;

		__m128 P0 = _mm_set_ps1(2.0f * s3 - 3.0f * s2 + 1.0f);
		__m128 T0 = _mm_set_ps1(s3 - 2.0f * s2 + s);
		__m128 P1 = _mm_set_ps1(-2.0f * s3 + 3.0f * s2);
		__m128 T1 = _mm_set_ps1(s3 - s2);

		__m128 vResult = _mm_mul_ps(P0, v1.vec);
		__m128 vTemp = _mm_mul_ps(T0, t1.vec);
		vResult = _mm_add_ps(vResult, vTemp);
		vTemp = _mm_mul_ps(P1, v2.vec);
		vResult = _mm_add_ps(vResult, vTemp);
		vTemp = _mm_mul_ps(T1, t2.vec);
		vResult = _mm_add_ps(vResult, vTemp);
		return vResult;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::lerp(const vec4 &v0, const vec4 &v1, float s)
	{
		return v0 + ((v1 - v0) * s);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::maximize(const vec4 &v0, const vec4 &v1)
	{
		return _mm_max_ps(v0.vec, v1.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::minimize(const vec4 &v0, const vec4 &v1)
	{
		return _mm_min_ps(v0.vec, v1.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::normalize(const vec4 &v)
	{
		if (vec4::equal3_all(v, vec4(0, 0, 0, 0))) return v;
		return _mm_div_ps(v.vec.vec, _mm_sqrt_ps(_mm_dp_ps(v.vec.vec, v.vec.vec, 0xFF)));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::normalizeapprox(const vec4 &v)
	{
		if (vec4::equal3_all(v, vec4(0, 0, 0, 0))) return v;
		return _mm_div_ps(v.vec.vec, _mm_rsqrt_ps(_mm_dp_ps(v.vec.vec, v.vec.vec, 0xFF)));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::normalize3(const vec4 &v)
	{
		if (vec4::equal3_all(v, vec4(0, 0, 0, 0))) return v;
		return _mm_div_ps(v.vec.vec, _mm_sqrt_ps(_mm_dp_ps(v.vec.vec, v.vec.vec, 0xF7)));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::normalizeapprox3(const vec4 &v)
	{
		if (vec4::equal3_all(v, vec4(0, 0, 0, 0))) return v;
		return _mm_div_ps(v.vec.vec, _mm_rsqrt_ps(_mm_dp_ps(v.vec.vec, v.vec.vec, 0xF7)));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::reflect(const vec4 &normal, const vec4 &incident)
	{
		__m128 res = _mm_mul_ps(incident.vec, normal.vec);
		res = _mm_add_ps(_mm_shuffle_ps(res, res, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_add_ps(_mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1)), _mm_shuffle_ps(res, res, _MM_SHUFFLE(2, 2, 2, 2))));
		res = _mm_add_ps(res, res);
		res = _mm_mul_ps(res, normal.vec);
		res = _mm_sub_ps(incident.vec, res);
		return res;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::less4_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res != 0xf;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::less4_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res == 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::lessequal4_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res != 0xf;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::lessequal4_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res == 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greater4_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greater4_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res == 0xf;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greaterequal4_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greaterequal4_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res == 0xf;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::equal4_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpeq_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::equal4_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpeq_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp);
		return res == 0xf;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::nearequal4(const vec4 &v0, const vec4 &v1, const vec4 &epsilon)
	{
		__m128 delta = _mm_sub_ps(v0.vec, v1.vec);
		__m128 temp = _mm_setzero_ps();
		temp = _mm_sub_ps(temp, delta);
		temp = _mm_max_ps(temp, delta);
		temp = _mm_cmple_ps(temp, epsilon.vec);
		return (_mm_movemask_ps(temp) == 0xf) != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::less3_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res != 7;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::less3_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res == 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::lessequal3_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res != 0x7;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::lessequal3_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res == 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greater3_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greater3_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpgt_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res == 0x7;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greaterequal3_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::greaterequal3_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpge_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res == 0x7;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::equal3_any(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpeq_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::equal3_all(const vec4 &v0, const vec4 &v1)
	{
		__m128 vTemp = _mm_cmpeq_ps(v0.vec, v1.vec);
		int res = _mm_movemask_ps(vTemp) & 7;
		return res == 0x7;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		vec4::nearequal3(const vec4 &v0, const vec4 &v1, const vec4 &epsilon)
	{
		__m128 delta = _mm_sub_ps(v0.vec, v1.vec);
		__m128 temp = _mm_setzero_ps();
		temp = _mm_sub_ps(temp, delta);
		temp = _mm_max_ps(temp, delta);
		temp = _mm_cmple_ps(temp, epsilon.vec);
		return (_mm_movemask_ps(temp) == 0x7) != 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::less(const vec4 &v0, const vec4 &v1)
	{
		return _mm_min_ps(_mm_cmplt_ps(v0.vec.vec, v1.vec.vec), _plus1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::greater(const vec4 &v0, const vec4 &v1)
	{
		return _mm_min_ps(_mm_cmpgt_ps(v0.vec.vec, v1.vec.vec), _plus1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::equal(const vec4 &v0, const vec4 &v1)
	{
		return _mm_min_ps(_mm_cmpeq_ps(v0.vec.vec, v1.vec.vec), _plus1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::unpack_x(__m128 v)
	{
		float x;
		_mm_store_ss(&x, v);
		return x;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::unpack_y(__m128 v)
	{
		float y;
		__m128 temp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
		_mm_store_ss(&y, temp);
		return y;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::unpack_z(__m128 v)
	{
		float y;
		__m128 temp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(&y, temp);
		return y;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::unpack_w(__m128 v)
	{
		float y;
		__m128 temp = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
		_mm_store_ss(&y, temp);
		return y;
	}
	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		vec4::splat(float s)
	{
		__m128 temp = _mm_set_ss(s);
		temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(0, 0, 0, 0));

		return temp;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		vec4::splat(const vec4 &v, uint element)
	{
		_assert(element < 4 && element >= 0, "Subscript out of range!\n");

		switch (element)
		{
		case 0:
			return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(0, 0, 0, 0));
		case 1:
			return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(1, 1, 1, 1));
		case 2:
			return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(2, 2, 2, 2));
		}
		return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(3, 3, 3, 3));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		vec4::splat_x(const vec4 &v)
	{
		return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(0, 0, 0, 0));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		vec4::splat_y(const vec4 &v)
	{
		return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(1, 1, 1, 1));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		vec4::splat_z(const vec4 &v)
	{
		return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(2, 2, 2, 2));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		vec4::splat_w(const vec4 &v)
	{
		return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(3, 3, 3, 3));
	}

	//------------------------------------------------------------------------------
	/**
	*/

	__forceinline vec4
		vec4::permute(const vec4& v0, const vec4& v1, unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3)
	{
		static mm128_ivec three = { 3, 3, 3, 3 };

		VORTEX_ALIGN16 unsigned int elem[4] = { i0, i1, i2, i3 };
		__m128i vControl = _mm_load_si128(reinterpret_cast<const __m128i *>(&elem[0]));

		__m128i vSelect = _mm_cmpgt_epi32(vControl, (reinterpret_cast<const __m128i *>(&three)[0]));
		vControl = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(vControl), three));

		__m128 shuffled1 = _mm_permutevar_ps(v0.vec, vControl);
		__m128 shuffled2 = _mm_permutevar_ps(v1.vec, vControl);

		__m128 masked1 = _mm_andnot_ps(_mm_castsi128_ps(vSelect), shuffled1);
		__m128 masked2 = _mm_and_ps(_mm_castsi128_ps(vSelect), shuffled2);

		return _mm_or_ps(masked1, masked2);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::floor(const vec4 &v)
	{
		return _mm_floor_ps(v.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::ceiling(const vec4 &v)
	{
		return _mm_ceil_ps(v.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::zerovector()
	{
		return vec4(0, 0, 0, 0);
	}
	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::upvec()
	{
		return vec4(0, 1, 0, 0);
	}
	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::rightvec()
	{
		return vec4(1, 0, 0, 0);
	}
	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4
		vec4::forwardvec()
	{
		return vec4(0, 0, 1, 0);
	}
	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		vec4::distancesq(Math::vec4 a, Math::vec4 b)
	{
		return _mm_cvtss_f32(_mm_dp_ps(a.vec, b.vec, 0xF1));
	}

} // namespace Math
//------------------------------------------------------------------------------








