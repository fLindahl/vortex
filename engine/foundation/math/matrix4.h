#pragma once
#include "config.h"
#include "quaternion.h"
#include "point.h"
#include "vector.h"
#include "plane.h"
#include "vector4.h"

#ifdef __GNUC__
#include <cmath>
#endif

//Macros
#define _mm_ror_ps(vec,i) (((i)%4) ? (_mm_shuffle_ps(vec,vec, _MM_SHUFFLE((unsigned char)(i+3)%4,(unsigned char)(i+2)%4,(unsigned char)(i+1)%4,(unsigned char)(i+0)%4))) : (vec))
#define _mm_rol_ps(vec,i) (((i)%4) ? (_mm_shuffle_ps(vec,vec, _MM_SHUFFLE((unsigned char)(7-i)%4,(unsigned char)(6-i)%4,(unsigned char)(5-i)%4,(unsigned char)(4-i)%4))) : (vec))

namespace Math
{
    typedef VORTEX_ALIGN16 struct mm128_mat
	{
		union
		{
			mm128_vec r[4];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	} mm128_mat;

	const mm128_ivec maskX = { -1, 0, 0, 0 };
	const mm128_ivec maskY = { 0, -1, 0, 0 };
	const mm128_ivec maskZ = { 0, 0, -1, 0 };
	const mm128_ivec maskW = { 0, 0, 0, -1 };

	// could not get the compiler to really pass it in registers for xbox, so
	// this is a reference so far
	typedef const mat4& __Matrix44Arg;

	class VORTEX_ALIGN16 mat4
	{
	public:
		/// default constructor, NOTE: does NOT setup components!
		mat4();
		/// construct from components
		mat4(vec4 const &row0, vec4 const &row1, vec4 const &row2, vec4 const &row3);
		mat4(const float&, const float&, const float&, const float&,
			 const float&, const float&, const float&, const float&,
			 const float&, const float&, const float&, const float&,
			 const float&, const float&, const float&, const float&);

		mat4(const float*);

		/// copy constructor
		//mat4(const mat4& rhs);
		/// construct from vmMat4
		mat4(const mm128_mat& rhs);

		/// assignment operator
		void operator=(const mat4& rhs);
		/// assign vmMat4
		void operator=(const mm128_mat& rhs);
		/// equality operator
		bool operator==(const mat4& rhs) const;
		/// inequality operator
		bool operator!=(const mat4& rhs) const;

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
		void set(vec4 const &row0, vec4 const &row1, vec4 const &row2, vec4 const &row3);
		/// write access to x component
		void setrow0(vec4 const &row0);
		/// write access to y component
		void setrow1(vec4 const &row1);
		/// write access to z component
		void setrow2(vec4 const &row2);
		/// write access to w component
		void setrow3(vec4 const &row3);
		/// read-only access to x component
		const vec4& getrow0() const;
		/// read-only access to y component
		const vec4& getrow1() const;
		/// read-only access to z component
		const vec4& getrow2() const;
		/// read-only access to w component
		const vec4& getrow3() const;
		/// read-only access to row
		const vec4& getrow(int row) const;
		/// read-write access to x component
		vec4& row0() const;
		/// read-write access to y component
		vec4& row1() const;
		/// read-write access to z component
		vec4& row2() const;
		/// read-write access to w component
		vec4& row3() const;
		/// read-write access to row
		vec4& row(int i);

		/// write access to x component
		void set_xaxis(vec4 const &x);
		/// write access to y component
		void set_yaxis(vec4 const &y);
		/// write access to z component
		void set_zaxis(vec4 const &z);
		/// write access to w component / pos component
		void set_position(vec4 const &pos);
		/// read access to x component
		const vec4& get_xaxis() const;
		/// read access to y component
		const vec4& get_yaxis() const;
		/// read access to z component
		const vec4& get_zaxis() const;
		/// read access to w component / pos component
		const vec4& get_position() const;
		/// extracts scale components to target vector
		void get_scale(vec4& scale) const;
		/// add a translation to pos_component
		void translate(vec4 const &t);
		/// scale matrix
		void scale(vec4 const &v);

		/// return true if matrix is identity
		bool isidentity() const;
		/// return determinant of matrix
		float determinant() const;
		/// decompose into scale, rotation and translation
		/// !!! Note:
		void decompose(vec4& outScale, quaternion& outRotation, vec4& outTranslation) const;

		/// build identity matrix
		static mat4 identity();
		/// build zero matrix (all elements are zero)
		static mat4 zeromatrix();
		/// build matrix from affine transformation
		static mat4 affinetransformation(float scaling, vec4 const &rotationCenter, const quaternion& rotation, vec4 const &translation);
		/// compute the inverse of a matrix
		static mat4 inverse(const mat4& m);
		/// build left handed lookat matrix
		static mat4 lookatlh(const point& eye, const point& at, const vector& up);
		/// build right handed lookat matrix
		static mat4 lookatrh(const point& eye, const point& at, const vector& up);
		/// multiply 2 matrices
		static mat4 multiply(const mat4& m0, const mat4& m1);
		/// build left handed orthogonal projection matrix
		static mat4 ortholh(float w, float h, float zn, float zf);
		/// build right handed orthogonal projection matrix
		static mat4 orthorh(float w, float h, float zn, float zf);
		/// build left-handed off-center orthogonal projection matrix
		static mat4 orthooffcenterlh(float l, float r, float b, float t, float zn, float zf);
		/// build right-handed off-center orthogonal projection matrix
		static mat4 orthooffcenterrh(float l, float r, float b, float t, float zn, float zf);
		/// build left-handed perspective projection matrix based on field-of-view
		static mat4 perspfovlh(float fovy, float aspect, float zn, float zf);
		/// build right-handed perspective projection matrix based on field-of-view
		static mat4 perspfovrh(float fovy, float aspect, float zn, float zf);
		/// build left-handed perspective projection matrix
		static mat4 persplh(float w, float h, float zn, float zf);
		/// build right-handed perspective projection matrix
		static mat4 persprh(float w, float h, float zn, float zf);
		/// build left-handed off-center perspective projection matrix
		static mat4 perspoffcenterlh(float l, float r, float b, float t, float zn, float zf);
		/// build right-handed off-center perspective projection matrix
		static mat4 perspoffcenterrh(float l, float r, float b, float t, float zn, float zf);
		/// build matrix that reflects coordinates about a plance
		static mat4 reflect(const plane& p);
		/// build rotation matrix around arbitrary axis
		static mat4 rotationaxis(vec4 const &axis, float angle);
		/// build rotation matrix from quaternion
		static mat4 rotationquaternion(const quaternion& q);
		/// build x-axis-rotation matrix
		static mat4 rotationx(float angle);
		/// build y-axis-rotation matrix
		static mat4 rotationy(float angle);
		/// build z-axis-rotation matrix
		static mat4 rotationz(float angle);
		/// build rotation matrix from yaw, pitch and roll
		static mat4 rotationyawpitchroll(float yaw, float pitch, float roll);
		/// build a scaling matrix from components
		static mat4 scaling(float sx, float sy, float sz);
		/// build a scaling matrix from vec4
		static mat4 scaling(vec4 const &s);
		/// build a transformation matrix
		static mat4 transformation(vec4 const &scalingCenter, const quaternion& scalingRotation, vec4 const &scaling, vec4 const &rotationCenter, const quaternion& rotation, vec4 const &translation);
		/// build a translation matrix
		static mat4 translation(float x, float y, float z);
		/// build a translation matrix from point
		static mat4 translation(vec4 const &t);
		/// return the transpose of a matrix
		static mat4 transpose(const mat4& m);
		/// transform 4d vector by mat4, faster inline version than vec4::transform
		static vec4 transform(const vec4 &v, const mat4 &m);
		/// return a quaternion from rotational part of the 4x4 matrix
		static quaternion rotationmatrix(const mat4& m);
		/// transform a plane with a matrix
		static plane transform(const plane& p, const mat4& m);
		/// check if point lies inside matrix frustum
		static bool ispointinside(const vec4& p, const mat4& m);
		/// convert to any type
		template<typename T> T as() const;

		///Prints the label + content of this matrix to the console
		///@todo	Move this to some debug class
		void print(const char* label);

		//private:
		friend class vec4;
		friend class plane;
		friend class quaternion;

		mm128_mat mat;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		mat4::mat4()
	{
		this->mat.r[0] = _id_x;
		this->mat.r[1] = _id_y;
		this->mat.r[2] = _id_z;
		this->mat.r[3] = _id_w;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		mat4::mat4(vec4 const &row0, vec4 const &row1, vec4 const &row2, vec4 const &row3)
	{
		this->mat.r[0].vec = row0.vec;
		this->mat.r[1].vec = row1.vec;
		this->mat.r[2].vec = row2.vec;
		this->mat.r[3].vec = row3.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
	mat4::mat4(const float& m0, const float& m1, const float& m2, const float& m3, const float& m4,
		const float& m5, const float& m6, const float& m7, const float& m8,
		const float& m9, const float& m10, const float& m11, const float& m12,
		const float& m13, const float& m14, const float& m15)
	{
		this->mat.r[0].vec = _mm_setr_ps(m0, m1, m2, m3);
		this->mat.r[1].vec = _mm_setr_ps(m4, m5, m6, m7);
		this->mat.r[2].vec = _mm_setr_ps(m8, m9, m10, m11);
		this->mat.r[3].vec = _mm_setr_ps(m12, m13, m14, m15);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		mat4::mat4(const float* m)
	{
		this->mat.r[0].vec = _mm_setr_ps(m[0], m[1], m[2], m[3]);
		this->mat.r[1].vec = _mm_setr_ps(m[4], m[5], m[6], m[7]);
		this->mat.r[2].vec = _mm_setr_ps(m[8], m[9], m[10], m[11]);
		this->mat.r[3].vec = _mm_setr_ps(m[12], m[13], m[14], m[15]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		mat4::mat4(const mm128_mat& rhs) :
		mat(rhs)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::operator=(const mat4& rhs)
	{
		this->mat = rhs.mat;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::operator=(const mm128_mat& rhs)
	{
		this->mat = rhs;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		mat4::operator==(const mat4& rhs) const
	{
		return vec4(this->mat.r[0]) == vec4(rhs.mat.r[0]) &&
			vec4(this->mat.r[1]) == vec4(rhs.mat.r[1]) &&
			vec4(this->mat.r[2]) == vec4(rhs.mat.r[2]) &&
			vec4(this->mat.r[3]) == vec4(rhs.mat.r[3]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		mat4::operator!=(const mat4& rhs) const
	{
		return !(*this == rhs);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::load(const float* ptr)
	{
		this->mat.r[0].vec = _mm_load_ps(ptr);
		this->mat.r[1].vec = _mm_load_ps(ptr + 4);
		this->mat.r[2].vec = _mm_load_ps(ptr + 8);
		this->mat.r[3].vec = _mm_load_ps(ptr + 12);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::loadu(const float* ptr)
	{
		this->mat.r[0].vec = _mm_loadu_ps(ptr);
		this->mat.r[1].vec = _mm_loadu_ps(ptr + 4);
		this->mat.r[2].vec = _mm_loadu_ps(ptr + 8);
		this->mat.r[3].vec = _mm_loadu_ps(ptr + 12);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::store(float* ptr) const
	{
		_mm_store_ps(ptr, this->mat.r[0].vec);
		_mm_store_ps((ptr + 4), this->mat.r[1].vec);
		_mm_store_ps((ptr + 8), this->mat.r[2].vec);
		_mm_store_ps((ptr + 12), this->mat.r[3].vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::storeu(float* ptr) const
	{
		_mm_storeu_ps(ptr, this->mat.r[0].vec);
		_mm_storeu_ps((ptr + 4), this->mat.r[1].vec);
		_mm_storeu_ps((ptr + 8), this->mat.r[2].vec);
		_mm_storeu_ps((ptr + 12), this->mat.r[3].vec);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::stream(float* ptr) const
	{
		this->storeu(ptr);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::set(vec4 const &row0, vec4 const &row1, vec4 const &row2, vec4 const &row3)
	{
		this->mat.r[0].vec = row0.vec;
		this->mat.r[1].vec = row1.vec;
		this->mat.r[2].vec = row2.vec;
		this->mat.r[3].vec = row3.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::setrow0(vec4 const &r)
	{
		this->mat.r[0].vec = r.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::getrow0() const
	{
		return *(vec4*)&(this->mat.r[0]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::setrow1(vec4 const &r)
	{
		this->mat.r[1] = r.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::getrow1() const
	{
		return *(vec4*)&(this->mat.r[1]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::setrow2(vec4 const &r)
	{
		this->mat.r[2] = r.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::getrow2() const
	{
		return *(vec4*)&(this->mat.r[2]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::setrow3(vec4 const &r)
	{
		this->mat.r[3] = r.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::getrow3() const
	{
		return *(vec4*)&(this->mat.r[3]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::getrow(int row) const
	{
		return *(vec4*)&(this->mat.r[row]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4&
		mat4::row0() const
	{
		return *(vec4*)&(this->mat.r[0]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4&
		mat4::row1() const
	{
		return *(vec4*)&(this->mat.r[1]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4&
		mat4::row2() const
	{
		return *(vec4*)&(this->mat.r[2]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4&
		mat4::row3() const
	{
		return *(vec4*)&(this->mat.r[3]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline vec4&
		mat4::row(int row)
	{
		return *(vec4*)&(this->mat.r[row]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::set_xaxis(vec4 const &x)
	{
		this->mat.r[0] = x.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::set_yaxis(vec4 const &y)
	{
		this->mat.r[1] = y.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::set_zaxis(vec4 const &z)
	{
		this->mat.r[2] = z.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::set_position(vec4 const &pos)
	{
		this->mat.r[3] = pos.vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::get_xaxis() const
	{
		return *(vec4*)&(this->mat.r[0]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::get_yaxis() const
	{
		return *(vec4*)&(this->mat.r[1]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::get_zaxis() const
	{
		return *(vec4*)&(this->mat.r[2]);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline const vec4&
		mat4::get_position() const
	{
		return *(vec4*)&(this->mat.r[3]);
	}


	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::get_scale(vec4& v) const
	{
		vec4 xaxis = this->mat.r[0];
		vec4 yaxis = this->mat.r[1];
		vec4 zaxis = this->mat.r[2];
		float xScale = xaxis.length3();
		float yScale = yaxis.length3();
		float zScale = zaxis.length3();

		v.set_x(xScale);
		v.set_y(yScale);
		v.set_z(zScale);
		v.set_w(1.0f);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		void
		mat4::translate(vec4 const &t)
	{
#if _DEBUG
		_assert2(t.w() == 0, "w component not 0, use vector for translation not a point!");
#endif
		this->mat.r[3] = (vec4(this->mat.r[3]) + t).vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline void
		mat4::scale(vec4 const &s)
	{
		// need to make sure that last column isn't erased
		vec4 scl = s;
		scl.set_w(1.0f);

		this->mat.r[0] = vec4::multiply(this->mat.r[0], scl).vec;
		this->mat.r[1] = vec4::multiply(this->mat.r[1], scl).vec;
		this->mat.r[2] = vec4::multiply(this->mat.r[2], scl).vec;
		this->mat.r[3] = vec4::multiply(this->mat.r[3], scl).vec;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline bool
		mat4::isidentity() const
	{
		return vec4(this->mat.r[0]) == _id_x &&
			vec4(this->mat.r[1]) == _id_y &&
			vec4(this->mat.r[2]) == _id_z &&
			vec4(this->mat.r[3]) == _id_w;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline float
		mat4::determinant() const
	{
		__m128 Va, Vb, Vc;
		__m128 r1, r2, r3, tt, tt2;
		__m128 sum, Det;

		__m128 _L1 = this->mat.r[0];
		__m128 _L2 = this->mat.r[1];
		__m128 _L3 = this->mat.r[2];
		__m128 _L4 = this->mat.r[3];
		// Calculating the minterms for the first line.

		// _mm_ror_ps is just a macro using _mm_shuffle_ps().
		tt = _L4; tt2 = _mm_ror_ps(_L3, 1);
		Vc = _mm_mul_ps(tt2, _mm_ror_ps(tt, 0));					// V3' dot V4
		Va = _mm_mul_ps(tt2, _mm_ror_ps(tt, 2));					// V3' dot V4"
		Vb = _mm_mul_ps(tt2, _mm_ror_ps(tt, 3));					// V3' dot V4^

		r1 = _mm_sub_ps(_mm_ror_ps(Va, 1), _mm_ror_ps(Vc, 2));		// V3" dot V4^ - V3^ dot V4"
		r2 = _mm_sub_ps(_mm_ror_ps(Vb, 2), _mm_ror_ps(Vb, 0));		// V3^ dot V4' - V3' dot V4^
		r3 = _mm_sub_ps(_mm_ror_ps(Va, 0), _mm_ror_ps(Vc, 1));		// V3' dot V4" - V3" dot V4'

		tt = _L2;
		Va = _mm_ror_ps(tt, 1);		sum = _mm_mul_ps(Va, r1);
		Vb = _mm_ror_ps(tt, 2);		sum = _mm_add_ps(sum, _mm_mul_ps(Vb, r2));
		Vc = _mm_ror_ps(tt, 3);		sum = _mm_add_ps(sum, _mm_mul_ps(Vc, r3));

		// Calculating the determinant.
		Det = _mm_mul_ps(sum, _L1);
		Det = _mm_add_ps(Det, _mm_movehl_ps(Det, Det));

		// Calculating the minterms of the second line (using previous results).
		tt = _mm_ror_ps(_L1, 1);		sum = _mm_mul_ps(tt, r1);
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));

		// Testing the determinant.
		Det = _mm_sub_ss(Det, _mm_shuffle_ps(Det, Det, 1));
		return vec4::unpack_x(Det);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::identity()
	{
		return mat4();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::zeromatrix()
	{
		mat4 mat;
		mat.mat.r[0] = _zero;
		mat.mat.r[1] = _zero;
		mat.mat.r[2] = _zero;
		mat.mat.r[3] = _zero;
		return mat;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::inverse(const mat4& m)
	{
		__m128 Va, Vb, Vc;
		__m128 r1, r2, r3, tt, tt2;
		__m128 sum, Det, RDet;
		__m128 trns0, trns1, trns2, trns3;

		const mm128_ivec pnpn = { 0x00000000, static_cast<int>(0x80000000), 0x00000000, static_cast<int>(0x80000000) };
		const mm128_ivec npnp = { static_cast<int>(0x80000000), 0x00000000, static_cast<int>(0x80000000), 0x00000000 };
		const mm128_vec zeroone = { 1.0f, 0.0f, 0.0f, 1.0f };

		__m128 _L1 = m.mat.r[0];
		__m128 _L2 = m.mat.r[1];
		__m128 _L3 = m.mat.r[2];
		__m128 _L4 = m.mat.r[3];
		// Calculating the minterms for the first line.

		// _mm_ror_ps is just a macro using _mm_shuffle_ps().
		tt = _L4; tt2 = _mm_ror_ps(_L3, 1);
		Vc = _mm_mul_ps(tt2, _mm_ror_ps(tt, 0));					// V3'dot V4
		Va = _mm_mul_ps(tt2, _mm_ror_ps(tt, 2));					// V3'dot V4"
		Vb = _mm_mul_ps(tt2, _mm_ror_ps(tt, 3));					// V3' dot V4^

		r1 = _mm_sub_ps(_mm_ror_ps(Va, 1), _mm_ror_ps(Vc, 2));		// V3" dot V4^ - V3^ dot V4"
		r2 = _mm_sub_ps(_mm_ror_ps(Vb, 2), _mm_ror_ps(Vb, 0));		// V3^ dot V4' - V3' dot V4^
		r3 = _mm_sub_ps(_mm_ror_ps(Va, 0), _mm_ror_ps(Vc, 1));		// V3' dot V4" - V3" dot V4'

		tt = _L2;
		Va = _mm_ror_ps(tt, 1);		sum = _mm_mul_ps(Va, r1);
		Vb = _mm_ror_ps(tt, 2);		sum = _mm_add_ps(sum, _mm_mul_ps(Vb, r2));
		Vc = _mm_ror_ps(tt, 3);		sum = _mm_add_ps(sum, _mm_mul_ps(Vc, r3));

		// Calculating the determinant.
		Det = _mm_mul_ps(sum, _L1);
		Det = _mm_add_ps(Det, _mm_movehl_ps(Det, Det));


		__m128 mtL1 = _mm_xor_ps(sum, pnpn);

		// Calculating the minterms of the second line (using previous results).
		tt = _mm_ror_ps(_L1, 1);		sum = _mm_mul_ps(tt, r1);
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));
		__m128 mtL2 = _mm_xor_ps(sum, npnp);

		// Testing the determinant.
		Det = _mm_sub_ss(Det, _mm_shuffle_ps(Det, Det, 1));

		// Calculating the minterms of the third line.
		tt = _mm_ror_ps(_L1, 1);
		Va = _mm_mul_ps(tt, Vb);									// V1' dot V2"
		Vb = _mm_mul_ps(tt, Vc);									// V1' dot V2^
		Vc = _mm_mul_ps(tt, _L2);								// V1' dot V2

		r1 = _mm_sub_ps(_mm_ror_ps(Va, 1), _mm_ror_ps(Vc, 2));		// V1" dot V2^ - V1^ dot V2"
		r2 = _mm_sub_ps(_mm_ror_ps(Vb, 2), _mm_ror_ps(Vb, 0));		// V1^ dot V2' - V1' dot V2^
		r3 = _mm_sub_ps(_mm_ror_ps(Va, 0), _mm_ror_ps(Vc, 1));		// V1' dot V2" - V1" dot V2'

		tt = _mm_ror_ps(_L4, 1);		sum = _mm_mul_ps(tt, r1);
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));
		__m128 mtL3 = _mm_xor_ps(sum, pnpn);

		// Dividing is FASTER than rcp_nr! (Because rcp_nr causes many register-memory RWs).
		RDet = _mm_div_ss(zeroone, Det); // TODO: just 1.0f?
		RDet = _mm_shuffle_ps(RDet, RDet, 0x00);

		// Devide the first 12 minterms with the determinant.
		mtL1 = _mm_mul_ps(mtL1, RDet);
		mtL2 = _mm_mul_ps(mtL2, RDet);
		mtL3 = _mm_mul_ps(mtL3, RDet);

		// Calculate the minterms of the forth line and devide by the determinant.
		tt = _mm_ror_ps(_L3, 1);		sum = _mm_mul_ps(tt, r1);
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
		tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));
		__m128 mtL4 = _mm_xor_ps(sum, npnp);
		mtL4 = _mm_mul_ps(mtL4, RDet);

		// Now we just have to transpose the minterms matrix.
		trns0 = _mm_unpacklo_ps(mtL1, mtL2);
		trns1 = _mm_unpacklo_ps(mtL3, mtL4);
		trns2 = _mm_unpackhi_ps(mtL1, mtL2);
		trns3 = _mm_unpackhi_ps(mtL3, mtL4);
		_L1 = _mm_movelh_ps(trns0, trns1);
		_L2 = _mm_movehl_ps(trns1, trns0);
		_L3 = _mm_movelh_ps(trns2, trns3);
		_L4 = _mm_movehl_ps(trns3, trns2);

		return mat4(vec4(_L1), vec4(_L2), vec4(_L3), vec4(_L4));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::lookatlh(const point& eye, const point& at, const vector& up)
	{
		// hmm the XM lookat functions are kinda pointless, because they
		// return a VIEW matrix, which is already inverse (so one would
		// need to reverse again!)
		const vec4 zaxis = vec4::normalize(at - eye);
		vec4 normUp = vec4::normalize(up);
		if (abs(vec4::dot3(zaxis, normUp)) > 0.9999999f)
		{
			// need to choose a different up vector because up and lookat point
			// into same or opposite direction
			// just rotate y->x, x->z and z->y
			normUp = vec4::permute(normUp, normUp, 1, 2, 0, 3);
		}
		const vec4 xaxis = vec4::normalize(vec4::cross3(normUp, zaxis));
		const vec4 yaxis = vec4::normalize(vec4::cross3(zaxis, xaxis));
		return mat4(xaxis.x(), yaxis.x(), zaxis.x(), 0.0f,
					xaxis.y(), yaxis.y(), zaxis.y(), 0.0f,
					xaxis.z(), yaxis.z(), zaxis.z(), 0.0f,
					-Math::vec4::dot(xaxis, eye), -Math::vec4::dot(yaxis, eye), -Math::vec4::dot(zaxis, eye), 1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::lookatrh(const point& eye, const point& at, const vector& up)
	{
		// hmm the XM lookat functions are kinda pointless, because they
		// return a VIEW matrix, which is already inverse (so one would
		// need to reverse again!)
		const vec4 zaxis = vec4::normalize(eye - at);
		vec4 normUp = vec4::normalize(up);
		if (abs(vec4::dot3(zaxis, normUp)) > 0.9999999f)
		{
			// need to choose a different up vector because up and lookat point
			// into same or opposite direction
			// just rotate y->x, x->z and z->y
			normUp = vec4::permute(normUp, normUp, 1, 2, 0, 3);
		}
		const vec4 xaxis = vec4::normalize(vec4::cross3(normUp, zaxis));
		const vec4 yaxis = vec4::normalize(vec4::cross3(zaxis, xaxis));

		return mat4(xaxis.x(), yaxis.x(), zaxis.x(), 0.0f,
					xaxis.y(), yaxis.y(), zaxis.y(), 0.0f,
					xaxis.z(), yaxis.z(), zaxis.z(), 0.0f,
					-Math::vec4::dot(xaxis, eye), -Math::vec4::dot(yaxis, eye), -Math::vec4::dot(zaxis, eye), 1);
	}

#ifdef N_USE_AVX
	// dual linear combination using AVX instructions on YMM regs
	static __forceinline __m256 twolincomb_AVX_8(__m256 A01, const mat4 &B)
	{
		__m256 result;
		result = _mm256_mul_ps(_mm256_shuffle_ps(A01, A01, 0x00), _mm256_broadcast_ps(&B.mat.r[0].vec));
		result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(A01, A01, 0x55), _mm256_broadcast_ps(&B.mat.r[1].vec)));
		result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(A01, A01, 0xaa), _mm256_broadcast_ps(&B.mat.r[2].vec)));
		result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(A01, A01, 0xff), _mm256_broadcast_ps(&B.mat.r[3].vec)));
		return result;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::multiply(const mat4& m0, const mat4& m1)
	{
		mat4 out;

		_mm256_zeroupper();
		__m256 A01 = _mm256_loadu_ps(&m0.mat.m[0][0]);
		__m256 A23 = _mm256_loadu_ps(&m0.mat.m[2][0]);

		__m256 out01x = twolincomb_AVX_8(A01, m1);
		__m256 out23x = twolincomb_AVX_8(A23, m1);

		_mm256_storeu_ps(&out.mat.m[0][0], out01x);
		_mm256_storeu_ps(&out.mat.m[2][0], out23x);
		return out;
	}
#else
	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::multiply(const mat4& m0, const mat4& m1)
	{
		mat4 ret;

		vec4 mw = m0.mat.r[0];

		// Splat the all components of the first row
		vec4 mx = vec4::splat_x(mw);
		vec4 my = vec4::splat_y(mw);
		vec4 mz = vec4::splat_z(mw);
		mw = vec4::splat_w(mw);

		vec4 m1x = m1.mat.r[0];
		vec4 m1y = m1.mat.r[1];
		vec4 m1z = m1.mat.r[2];
		vec4 m1w = m1.mat.r[3];

		//multiply first row
		mx = vec4::multiply(mx, m1x);
		my = vec4::multiply(my, m1y);
		mz = vec4::multiply(mz, m1z);
		mw = vec4::multiply(mw, m1w);

		mx = mx + my;
		mz = mz + mw;
		ret.mat.r[0] = (mx + mz).vec;

		// rinse and repeat
		mw = m0.getrow1();

		mx = vec4::splat_x(mw);
		my = vec4::splat_y(mw);
		mz = vec4::splat_z(mw);
		mw = vec4::splat_w(mw);

		mx = vec4::multiply(mx, m1x);
		my = vec4::multiply(my, m1y);
		mz = vec4::multiply(mz, m1z);
		mw = vec4::multiply(mw, m1w);

		mx = mx + my;
		mz = mz + mw;
		ret.mat.r[1] = (mx + mz).vec;

		mw = m0.getrow2();

		mx = vec4::splat_x(mw);
		my = vec4::splat_y(mw);
		mz = vec4::splat_z(mw);
		mw = vec4::splat_w(mw);

		mx = vec4::multiply(mx, m1.mat.r[0]);
		my = vec4::multiply(my, m1.mat.r[1]);
		mz = vec4::multiply(mz, m1.mat.r[2]);
		mw = vec4::multiply(mw, m1.mat.r[3]);

		mx = mx + my;
		mz = mz + mw;
		ret.mat.r[2] = (mx + mz).vec;

		mw = m0.getrow3();

		mx = vec4::splat_x(mw);
		my = vec4::splat_y(mw);
		mz = vec4::splat_z(mw);
		mw = vec4::splat_w(mw);

		mx = vec4::multiply(mx, m1x);
		my = vec4::multiply(my, m1y);
		mz = vec4::multiply(mz, m1z);
		mw = vec4::multiply(mw, m1w);

		mx = mx + my;
		mz = mz + mw;
		ret.mat.r[3] = (mx + mz).vec;

		return ret;
	}
#endif

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::ortholh(float w, float h, float zn, float zf)
	{
		mat4 m;
		float dist = 1.0f / (zf - zn);
		m.setrow0(vec4(2.0f / w, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f / h, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, 0.0f));
		m.setrow3(vec4(0.0f, 0.0, -dist * zn, 1.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::orthorh(float w, float h, float zn, float zf)
	{
		mat4 m;
		float dist = 1.0f / (zn - zf);
		m.setrow0(vec4(2.0f / w, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f / h, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, 0.0f));
		m.setrow3(vec4(0.0f, 0.0, dist * zn, 1.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::orthooffcenterlh(float l, float r, float b, float t, float zn, float zf)
	{
		mat4 m;
		float divwidth = 1.0f / (r - l);
		float divheight = 1.0f / (t - b);
		float dist = 1.0f / (zf - zn);

		m.setrow0(vec4(2.0f * divwidth, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f * divheight, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, 0.0f));
		m.setrow3(vec4(-(l + r) * divwidth, -(b + t) * divheight, -dist *  zn, 1.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::orthooffcenterrh(float l, float r, float b, float t, float zn, float zf)
	{
		mat4 m;
		float divwidth = 1.0f / (r - l);
		float divheight = 1.0f / (t - b);
		float dist = 1.0f / (zn - zf);

		m.setrow0(vec4(2.0f * divwidth, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f * divheight, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, 0.0f));
		m.setrow3(vec4(-(l + r) * divwidth, -(b + t) * divheight, dist *  zn, 1.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::perspfovlh(float fovy, float aspect, float zn, float zf)
	{
		mat4 m;
		float halfFov = 0.5f * fovy;
		float sinfov = sinf(halfFov);
		float cosfov = cosf(halfFov);

		float height = cosfov / sinfov;
		float width = height / aspect;

		float dist = zf / (zf - zn);

		m.setrow0(vec4(width, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, height, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, 1.0f));
		m.setrow3(vec4(0.0f, 0.0f, -dist * zn, 0.0f));

		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::perspfovrh(float fovy, float aspect, float zn, float zf)
	{
		mat4 m;
		float halfFov = 0.5f * fovy;
		float sinfov = sinf(halfFov);
		float cosfov = cosf(halfFov);

		float height = cosfov / sinfov;
		float width = height / aspect;

		float dist = zf / (zn - zf);

		m.setrow0(vec4(width, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, height, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, -1.0f));
		m.setrow3(vec4(0.0f, 0.0f, dist * zn, 0.0f));

		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::persplh(float w, float h, float zn, float zf)
	{
		mat4 m;
		float dist = zf / (zf - zn);
		m.setrow0(vec4(2.0f * zn / w, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f * zn / h, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, 1.0f));
		m.setrow3(vec4(0.0f, 0.0, -dist * zn, 0.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::persprh(float w, float h, float zn, float zf)
	{
		mat4 m;
		float dist = zf / (zn - zf);
		m.setrow0(vec4(2.0f * zn / w, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f * zn / h, 0.0f, 0.0f));
		m.setrow2(vec4(0.0f, 0.0f, dist, -1.0f));
		m.setrow3(vec4(0.0f, 0.0, dist * zn, 0.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::perspoffcenterlh(float l, float r, float b, float t, float zn, float zf)
	{
		mat4 m;
		float divwidth = 1.0f / (r - l);
		float divheight = 1.0f / (t - b);
		float dist = zf / (zf - zn);

		m.setrow0(vec4(2.0f * zn * divwidth, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f * zn * divheight, 0.0f, 0.0f));
		m.setrow2(vec4(-(l + r) * divwidth, -(b + t) * divheight, dist, 1.0f));
		m.setrow3(vec4(0.0f, 0.0f, -dist * zn, 0.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::perspoffcenterrh(float l, float r, float b, float t, float zn, float zf)
	{
		mat4 m;
		float divwidth = 1.0f / (r - l);
		float divheight = 1.0f / (t - b);
		float dist = zf / (zn - zf);

		m.setrow0(vec4(2.0f * zn * divwidth, 0.0f, 0.0f, 0.0f));
		m.setrow1(vec4(0.0f, 2.0f * zn * divheight, 0.0f, 0.0f));
		m.setrow2(vec4((l + r) * divwidth, (b + t) * divheight, dist, -1.0f));
		m.setrow3(vec4(0.0f, 0.0f, dist * zn, 0.0f));
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::rotationaxis(vec4 const &axis, float angle)
	{
		__m128 norm = vec4::normalize3(axis).vec;

		float sangle = sinf(angle);
		float cangle = cosf(angle);

		__m128 m1_c = _mm_set_ps1(1.0f - cangle);
		__m128 c = _mm_set_ps1(cangle);
		__m128 s = _mm_set_ps1(sangle);

		__m128 nn1 = _mm_shuffle_ps(norm, norm, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 nn2 = _mm_shuffle_ps(norm, norm, _MM_SHUFFLE(3, 1, 0, 2));

		__m128 v = _mm_mul_ps(nn1, m1_c);
		v = _mm_mul_ps(nn2, v);

		__m128 nn3 = _mm_mul_ps(norm, m1_c);
		nn3 = _mm_mul_ps(norm, nn3);
		nn3 = _mm_add_ps(nn3, c);

		__m128 nn4 = _mm_mul_ps(norm, s);
		nn4 = _mm_add_ps(nn4, v);
		__m128 nn5 = _mm_mul_ps(s, norm);
		nn5 = _mm_sub_ps(v, nn5);

		const mm128_ivec mask = { -1, -1, -1, 0 };
		v = _mm_and_ps(nn3, mask);

		__m128 v1 = _mm_shuffle_ps(nn4, nn5, _MM_SHUFFLE(2, 1, 2, 0));
		v1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 3, 2, 1));

		__m128 v2 = _mm_shuffle_ps(nn4, nn5, _MM_SHUFFLE(0, 0, 1, 1));
		v2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 0, 2, 0));


		nn5 = _mm_shuffle_ps(v, v1, _MM_SHUFFLE(1, 0, 3, 0));
		nn5 = _mm_shuffle_ps(nn5, nn5, _MM_SHUFFLE(1, 3, 2, 0));

		mat4 m;
		m.setrow0(nn5);

		nn5 = _mm_shuffle_ps(v, v1, _MM_SHUFFLE(3, 2, 3, 1));
		nn5 = _mm_shuffle_ps(nn5, nn5, _MM_SHUFFLE(1, 3, 0, 2));
		m.setrow1(nn5);

		v2 = _mm_shuffle_ps(v2, v, _MM_SHUFFLE(3, 2, 1, 0));

		m.setrow2(v2);

		m.mat.r[3] = _id_w;
		return m;


	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::rotationx(float angle)
	{
		mat4 m;

		float sangle = sinf(angle);
		float cangle = cosf(angle);

		m.mat.m[1][1] = cangle;
		m.mat.m[1][2] = sangle;

		m.mat.m[2][1] = -sangle;
		m.mat.m[2][2] = cangle;
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::rotationy(float angle)
	{
		mat4 m;

		float sangle = sinf(angle);
		float cangle = cosf(angle);

		m.mat.m[0][0] = cangle;
		m.mat.m[0][2] = -sangle;

		m.mat.m[2][0] = sangle;
		m.mat.m[2][2] = cangle;
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::rotationz(float angle)
	{
		mat4 m;

		float sangle = sinf(angle);
		float cangle = cosf(angle);

		m.mat.m[0][0] = cangle;
		m.mat.m[0][1] = sangle;

		m.mat.m[1][0] = -sangle;
		m.mat.m[1][1] = cangle;
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::rotationyawpitchroll(float yaw, float pitch, float roll)
	{
		quaternion q = quaternion::rotationyawpitchroll(yaw, pitch, roll);
		return mat4::rotationquaternion(q);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::scaling(float sx, float sy, float sz)
	{
		mat4 m;
		m.mat.r[0].vec = _mm_set_ps(0.0f, 0.0f, 0.0f, sx);
		m.mat.r[1].vec = _mm_set_ps(0.0f, 0.0f, sy, 0.0f);
		m.mat.r[2].vec = _mm_set_ps(0.0f, sz, 0.0f, 0.f);
		m.mat.r[3].vec = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::scaling(vec4 const &s)
	{
		mat4 m;
		m.mat.r[0].vec = _mm_and_ps(s.vec, maskX);
		m.mat.r[1].vec = _mm_and_ps(s.vec, maskY);
		m.mat.r[2].vec = _mm_and_ps(s.vec, maskZ);

		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::translation(float x, float y, float z)
	{
		mat4 m;
		m.mat.r[3].vec = _mm_set_ps(1.0f, z, y, x);
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::translation(vec4 const &t)
	{
		mat4 m;
		m.mat.r[3].vec = t.vec;
		m.mat._44 = 1.0f;
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline mat4
		mat4::transpose(const mat4& m)
	{
		__m128 xy1 = _mm_shuffle_ps(m.mat.r[0].vec, m.mat.r[1].vec, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 zw1 = _mm_shuffle_ps(m.mat.r[0].vec, m.mat.r[1].vec, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 xy2 = _mm_shuffle_ps(m.mat.r[2].vec, m.mat.r[3].vec, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 zw2 = _mm_shuffle_ps(m.mat.r[2].vec, m.mat.r[3].vec, _MM_SHUFFLE(3, 2, 3, 2));

		mat4 r;
		r.mat.r[0].vec = _mm_shuffle_ps(xy1, xy2, _MM_SHUFFLE(2, 0, 2, 0));
		r.mat.r[1].vec = _mm_shuffle_ps(xy1, xy2, _MM_SHUFFLE(3, 1, 3, 1));
		r.mat.r[2].vec = _mm_shuffle_ps(zw1, zw2, _MM_SHUFFLE(2, 0, 2, 0));
		r.mat.r[3].vec = _mm_shuffle_ps(zw1, zw2, _MM_SHUFFLE(3, 1, 3, 1));
		return r;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		vec4
		mat4::transform(const vec4 &v, const mat4 &m)
	{
		__m128 x = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 y = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 z = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 w = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(3, 3, 3, 3));

		return _mm_add_ps(
			_mm_add_ps(_mm_mul_ps(x, m.mat.r[0].vec), _mm_mul_ps(y, m.mat.r[1].vec)),
			_mm_add_ps(_mm_mul_ps(z, m.mat.r[2].vec), _mm_mul_ps(w, m.mat.r[3].vec))
			);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		quaternion
		mat4::rotationmatrix(const mat4& m)
	{
		float trace = m.mat.m[0][0] + m.mat.m[1][0] + m.mat.m[2][0];
		mat4 mm = mat4::transpose(m);
		float temp[4];

		if (trace > 0.0f)
		{
			float s = sqrt(trace + 1.0f);
			temp[3] = (s * 0.5f);
			s = 0.5f / s;

			temp[0] = ((mm.mat._32 - mm.mat._23) * s);
			temp[1] = ((mm.mat._13 - mm.mat._31) * s);
			temp[2] = ((mm.mat._21 - mm.mat._12) * s);
		}
		else
		{
			int i = mm.mat._11 < mm.mat._22 ?
				(mm.mat._22 < mm.mat._33 ? 2 : 1) :
				(mm.mat._11 < mm.mat._33 ? 2 : 0);
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;

			float s = sqrt(mm.mat.m[i][i] - mm.mat.m[j][j] - mm.mat.m[k][k] + 1.0f);
			temp[i] = s * 0.5f;
			s = 0.5f / s;

			//Find correct row / column
			// y + x * 4 
			temp[3] = (mm.mat.m[k][j] - mm.mat.m[j][k]) * s;
			temp[j] = (mm.mat.m[j][i] + mm.mat.m[i][j]) * s;
			temp[k] = (mm.mat.m[k][i] + mm.mat.m[i][k]) * s;
		}
		quaternion q(temp[0], temp[1], temp[2], temp[3]);
		return q;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	__forceinline
		plane
		mat4::transform(const plane &p, const mat4& m)
	{
		printf("not implemented! /n");
		assert(false);
		return plane(vec4(), vec4(), vec4());//mat4::transform(p.vec, m);
	}

} // namespace Math
//------------------------------------------------------------------------------

