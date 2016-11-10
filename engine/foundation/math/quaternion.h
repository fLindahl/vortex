#pragma once

#include "config.h"
#include "vector4.h"

#ifndef WIN32
#define BT_USE_SSE
#endif

//------------------------------------------------------------------------------
namespace Math
{
	class quaternion;
	
#if (defined(_XM_VMX128_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_))
	typedef const quaternion  __QuaternionArg;
#else
	// win32 VC++ does not support passing aligned objects as value so far
	// here is a bug-report at microsoft saying so:
	// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=334581
	typedef const quaternion& __QuaternionArg;
#endif


	class VORTEX_ALIGN16 quaternion
	{
	public:
		/// default constructor, NOTE: does NOT setup components!
		quaternion();
		/// construct from components
		quaternion(float x, float y, float z, float w);
		/// construct from vec4
		quaternion(vec4 const &rhs);
		/// copy constructor
		/// !!!! copy constructor forbidden, otherwise passing point's to a function
		/// !!!! via Registers doesnt work
		//quaternion(const quaternion& rhs);
		/// construct from __m128
		quaternion(const __m128 & rhs);
		/// construct from a Quat
		quaternion(const quaternion& rhs);

		/// assignment operator
		void operator=(const quaternion& rhs);
		/// assign __m128
		void operator=(const __m128 & rhs);
		/// equality operator
		bool operator==(const quaternion& rhs) const;
		/// inequality operator
		bool operator!=(const quaternion& rhs) const;

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

		/// set content
		void set(float x, float y, float z, float w);
		/// set from vec4
		void set(vec4 const &f4);
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

		/// return true if quaternion is identity
		bool isidentity() const;
		/// returns length
		float length() const;
		/// returns length squared
		float lengthsq() const;
		/// un-denormalize quaternion (this is sort of a hack since Maya likes to return denormal quaternions)
		void undenormalize();

		/// return quaternion in barycentric coordinates
		static quaternion barycentric(const quaternion& q0, const quaternion& q1, const quaternion& q2, float f, float g);
		/// return conjugate of a normalized quaternion
		static quaternion conjugate(const quaternion& q);
		/// return dot product of two normalized quaternions
		static float dot(const quaternion& q0, const quaternion& q1);
		/// calculate the exponential
		static quaternion exp(const quaternion& q0);
		/// returns an identity quaternion
		static quaternion identity();
		/// conjugates and renormalizes quaternion
		static quaternion inverse(const quaternion& q);
		/// calculate the natural logarithm
		static quaternion ln(const quaternion& q);
		/// multiply 2 quaternions
		static quaternion multiply(const quaternion& q0, const quaternion& q1);
		/// compute unit length quaternion
		static quaternion normalize(const quaternion& q);
		/// build quaternion from axis and clockwise rotation angle in radians
		static quaternion rotationaxis(const vec4& axis, float angle);
		/// build quaternion from rotation matrix
		static quaternion rotationmatrix(const mat4& m);
		/// build quaternion from yaw, pitch and roll
		static quaternion rotationyawpitchroll(float yaw, float pitch, float roll);
		/// interpolate between 2 quaternion using spherical interpolation
		static quaternion slerp(const quaternion& q1, const quaternion& q2, float t);
		/// setup control points for spherical quadrangle interpolation
		static void squadsetup(const quaternion& q0, const quaternion& q1, const quaternion& q2, const quaternion& q3, quaternion& aOut, quaternion& bOut, quaternion& cOut);
		/// interpolate between quaternions using spherical quadrangle interpolation
		static quaternion squad(const quaternion& q1, const quaternion& a, const quaternion& b, const quaternion& c, float t);
		/// convert quaternion to axis and angle
		static void to_axisangle(const quaternion& q, vec4& outAxis, float& outAngle);

	private:
		friend class mat4;

		vec4 vec;

	};

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		quaternion::quaternion()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		quaternion::quaternion(float x, float y, float z, float w)
	{
		this->vec = vec4(x, y, z, w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		quaternion::quaternion(vec4 const &rhs) :
		vec(rhs.vec.vec)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		quaternion::quaternion(const __m128 & rhs) :
		vec(rhs)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		quaternion::quaternion(const quaternion& rhs) :
		vec(rhs.vec)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::operator=(const quaternion& rhs)
	{
		this->vec = rhs.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::operator=(const __m128 & rhs)
	{
		this->vec = vec4(rhs);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		quaternion::operator==(const quaternion& rhs) const
	{
		return rhs.vec == this->vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		quaternion::operator!=(const quaternion& rhs) const
	{
		return !(this->operator==(rhs));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::load(const float* ptr)
	{
		vec4 temp;
		temp.load(ptr);
		this->vec = vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::loadu(const float* ptr)
	{
		vec4 temp;
		temp.loadu(ptr);
		this->vec = temp;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::store(float* ptr) const
	{
		vec4 temp(vec);
		temp.store(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::storeu(float* ptr) const
	{
		vec4 temp(vec);
		temp.storeu(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::stream(float* ptr) const
	{
		this->store(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::set(float x, float y, float z, float w)
	{
		this->vec = vec4(x, y, z, w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		quaternion::set_x(float x)
	{
		this->vec.set_x(x);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		quaternion::set_y(float y)
	{
		this->vec.set_y(y);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		quaternion::set_z(float z)
	{
		this->vec.set_z(z);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		quaternion::set_w(float w)
	{
		this->vec.set_w(w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::set(vec4 const &f4)
	{
		this->vec = f4;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		quaternion::x()
	{
		return vec.x();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::x() const
	{
		return this->vec.x();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		quaternion::y()
	{
		return vec.y();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::y() const
	{
		return this->vec.y();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		quaternion::z()
	{
		return vec.z();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::z() const
	{
		return this->vec.z();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float&
		quaternion::w()
	{
		return vec.w();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::w() const
	{
		return this->vec.w();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		quaternion::isidentity() const
	{
		const quaternion id(0, 0, 0, 1);
		return id == *this;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::length() const
	{
		return (this->vec.length());
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::lengthsq() const
	{
		return this->vec.lengthsq();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::undenormalize()
	{
		printf("UNDENOMALIZE NOT IMPLEMENTED\n");
		assert(false);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::barycentric(const quaternion& q0, const quaternion& q1, const quaternion& q2, float f, float g)
	{
		float s = f + g;
		if (s != 0.0f)
		{
			quaternion a = quaternion::slerp(q0, q1, s);
			quaternion b = quaternion::slerp(q0, q2, s);
			quaternion res = quaternion::slerp(a, b, g / s);
			return res;
		}
		else
		{
			return q0;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::conjugate(const quaternion& q)
	{
		const VORTEX_ALIGN16 mm128_vec con = { -1.0f, -1.0f, -1.0f, 1.0f };
		quaternion qq(_mm_mul_ps(q.vec.vec, con.vec));
		return qq;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		quaternion::dot(const quaternion& q0, const quaternion& q1)
	{
		return vec4::dot(q0.vec, q1.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::exp(const quaternion& q)
	{
		vec4 f(q.vec.vec);
		float theta = f.length3();
		float costheta = cosf(theta);
		float sintheta = sinf(theta);

		f *= sintheta / theta;
		f.set_w(costheta);

		return quaternion(f.vec.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::identity()
	{
		return quaternion(0,0,0,1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::inverse(const quaternion& q)
	{
		float len = q.lengthsq();
		if (len > 0.00001f)
		{
			quaternion con = quaternion::conjugate(q);
			con.vec *= 1.0f / len;
			return con;
		}
		return quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::ln(const quaternion& q)
	{

		quaternion ret;

		float a = cosf(q.w());
		float isina = 1.0f / sinf(a);

		float aisina = a * isina;
		if (isina > 0)
		{
			ret.set(q.x()*aisina, q.y()*aisina, q.z()*aisina, 0.0f);
		}
		else {
			ret.set(0.0f, 0.0f, 0.0f, 0.0f);
		}
		return ret;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::multiply(const quaternion& q0, const quaternion& q1)
	{
		float x = q0.x() * q1.w() + q0.y() * q1.z() - q0.z() * q1.y() + q0.w() * q1.x();
		float y = -q0.x() * q1.z() + q0.y() * q1.w() + q0.z() * q1.x() + q0.w() * q1.y();
		float z = q0.x() * q1.y() - q0.y() * q1.x() + q0.z() * q1.w() + q0.w() * q1.z();
		float w = -q0.x() * q1.x() - q0.y() * q1.y() - q0.z() * q1.z() + q0.w() * q1.w();

		return quaternion(x,y,z,w);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::normalize(const quaternion& q)
	{
		return vec4::normalize(q.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::rotationaxis(const vec4& axis, float angle)
	{
		printf("quaternion::rotationaxis: Not implemented!/n");
		assert(false);
		return quaternion(0, 0, 0, 0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::rotationyawpitchroll(float yaw, float pitch, float roll)
	{

		float halfYaw = 0.5f * yaw;
		float halfPitch = 0.5f * pitch;
		float halfRoll = 0.5f * roll;
		float cosYaw = cosf(halfYaw);
		float sinYaw = sinf(halfYaw);
		float cosPitch = cosf(halfPitch);
		float sinPitch = sinf(halfPitch);
		float cosRoll = cosf(halfRoll);
		float sinRoll = sinf(halfRoll);
		quaternion q(-(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw),
			-(cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw),
			-(sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw),
			-(cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw));


		return q;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::slerp(const quaternion& q0, const quaternion& q1, float t)
	{
		quaternion q = quaternion();

		double cosHalfTheta = q0.x()*q1.x() + q0.y()*q1.y() + q0.z()*q1.z() + q0.w()*q1.w();
		// if qa=qb or qa=-qb then theta = 0 and we can return qa
		if (abs(cosHalfTheta) == 1.0)
		{
			return q0;
		}
		bool reverse_q1 = false;
		if (cosHalfTheta < 0) // Always follow the shortest path
		{
			reverse_q1 = true;
			cosHalfTheta = -cosHalfTheta;
		}
		// Calculate temporary values.
		const double halfTheta = acos(cosHalfTheta);
		const double sinHalfTheta = std::sqrt(1.0 - (cosHalfTheta * cosHalfTheta));
		// if theta = 180 degrees then result is not fully defined
		// we could rotate around any axis normal to qa or qb
		if (std::abs(sinHalfTheta) < 0.001)
		{
			if (!reverse_q1)
			{
				q.vec.x() = (1 - t)*q0.vec.x() + t*q1.vec.x();
				q.vec.y() = (1 - t)*q0.vec.y() + t*q1.vec.y();
				q.vec.z() = (1 - t)*q0.vec.z() + t*q1.vec.z();
				q.vec.w() = (1 - t)*q0.vec.w() + t*q1.vec.w();
			}
			else
			{
				q.vec.x() = (1 - t)*q0.vec.x() - t*q1.vec.x();
				q.vec.y() = (1 - t)*q0.vec.y() - t*q1.vec.y();
				q.vec.z() = (1 - t)*q0.vec.z() - t*q1.vec.z();
				q.vec.w() = (1 - t)*q0.vec.w() - t*q1.vec.w();
			}
			return q;
		}
		const double A = sin((1 - t) * halfTheta) / sinHalfTheta;
		const double B = sin(t*halfTheta) / sinHalfTheta;
		if (!reverse_q1)
		{
			q.vec.x() = A*q0.vec.x() + B*q1.vec.x();
			q.vec.y() = A*q0.vec.y() + B*q1.vec.y();
			q.vec.z() = A*q0.vec.z() + B*q1.vec.z();
			q.vec.w() = A*q0.vec.w() + B*q1.vec.w();
		}
		else
		{
			q.vec.x() = A*q0.vec.x() - B*q1.vec.x();
			q.vec.y() = A*q0.vec.y() - B*q1.vec.y();
			q.vec.z() = A*q0.vec.z() - B*q1.vec.z();
			q.vec.w() = A*q0.vec.w() - B*q1.vec.w();
		}
		return q;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::squadsetup(const quaternion& q0, const quaternion& q1, const quaternion& q2, const quaternion& q3, quaternion& aOut, quaternion& bOut, quaternion& cOut)
	{
		printf("quaternion::squadsetup: Not implemented!/n");
		assert(false);
		//n_error("not implemented");
		//XMQuaternionSquadSetup(&aOut.vec, &bOut.vec, &cOut.vec, q0.vec, q1.vec, q2.vec, q3.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline quaternion
		quaternion::squad(const quaternion& q1, const quaternion& a, const quaternion& b, const quaternion& c, float t)
	{
		printf("quaternion::squad: Not implemented!/n");
		assert(false);
		return quaternion(0, 0, 0, 0);
		//return Vectormath::Aos::squad(t, q1.vec, a.vec, b.vec, c.vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		quaternion::to_axisangle(const quaternion& q, vec4& outAxis, float& outAngle)
	{
		outAxis = q.vec;
		outAxis.set_w(0);
		outAngle = 2.0f * cosf(q.vec.w());
		outAxis.set_w(0.0f);
	}

} // namespace Math
//------------------------------------------------------------------------------
