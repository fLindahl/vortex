#include "config.h"
#include "math.h"
#include "matrix4.h"
#include "plane.h"
#include "quaternion.h"
#include "vector4.h"

namespace Math
{
	//------------------------------------------------------------------------------
	/**
	based on this http://www.opengl.org/discussion_boards/showthread.php/169605-reflection-matrix-how-to-derive
	*/
	mat4
	mat4::reflect(const plane& p)
	{
		vec4 norm = vec4::normalize(p.vec);

		const vec4 two(-2.0f, -2.0f, -2.0f, 0.0f);

		// s = -2 * n
		vec4 s = vec4::multiply(norm, two);

		vec4 x = vec4::splat_x(norm);
		vec4 y = vec4::splat_y(norm);
		vec4 z = vec4::splat_z(norm);
		vec4 w = vec4::splat_w(norm);

		mat4 m;
		// identity - 2 * nxn
		m.setrow0(vec4::multiplyadd(x, s, _id_x));
		m.setrow1(vec4::multiplyadd(y, s, _id_y));
		m.setrow2(vec4::multiplyadd(z, s, _id_z));
		m.setrow3(vec4::multiplyadd(w, s, _id_w));

		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	mat4::decompose(vec4& outScale, quaternion& outRotation, vec4& outTranslation) const
	{
		// Copy the matrix first - we'll use this to break down each component
		mat4 mCopy(this->mat);

		// Start by extracting the translation (and/or any projection) from the given matrix
		outTranslation = mCopy.get_position();
		outTranslation.set_w(0.0f);
		mCopy.set_position(_id_w);

		// Extract the rotation component - this is done using polar decompostion, where
		// we successively average the matrix with its inverse transpose until there is
		// no/a very small difference between successive averages
		float norm;
		int count = 0;
		mat4 rotation = mCopy;
		do {
			mat4 nextRotation;
			mat4 currInvTranspose = mat4::inverse(mat4::transpose(rotation));

			// Go through every component in the matrices and find the next matrix		
			nextRotation.setrow0((rotation.getrow0() + currInvTranspose.getrow0()) * 0.5f);
			nextRotation.setrow1((rotation.getrow1() + currInvTranspose.getrow1()) * 0.5f);
			nextRotation.setrow2((rotation.getrow2() + currInvTranspose.getrow2()) * 0.5f);
			nextRotation.setrow3((rotation.getrow3() + currInvTranspose.getrow3()) * 0.5f);

			norm = 0.0f;
			norm = max(norm, vec4::dot3((rotation.getrow0() - nextRotation.getrow0()).abs(), _plus1));
			norm = max(norm, vec4::dot3((rotation.getrow1() - nextRotation.getrow1()).abs(), _plus1));
			norm = max(norm, vec4::dot3((rotation.getrow2() - nextRotation.getrow2()).abs(), _plus1));

			rotation = nextRotation;
		} while (count < 100 && norm > 0.00001f);

		outRotation = mat4::rotationmatrix(rotation);

		// The scale is simply the removal of the rotation from the non-translated matrix
		mat4 scaleMatrix = mat4::multiply(mCopy, mat4::inverse(rotation));
		scaleMatrix.get_scale(outScale);
		outScale.set_w(0.0f);

		// Calculate the normalized rotation matrix and take its determinant to determine whether
		// it had a negative scale or not...
		vec4 r0 = vec4::normalize(mCopy.getrow0());
		vec4 r1 = vec4::normalize(mCopy.getrow1());
		vec4 r2 = vec4::normalize(mCopy.getrow2());
		mat4 nr(r0, r1, r2, _id_w);

		// Special consideration: if there's a single negative scale 
		// (all other combinations of negative scales will
		// be part of the rotation matrix), the determinant of the 
		// normalized rotation matrix will be < 0. 
		// If this is the case we apply an arbitrary negative to one 
		// of the component of the scale.
		float determinant = nr.determinant();
		if (determinant < 0.0)
		{
			outScale.set_x(outScale.x() * -1.0f);
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	mat4
	mat4::affinetransformation(float scaling, vec4 const &rotationCenter, const quaternion& rotation, vec4 const &translation)
	{
		// M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;
		vec4 scalev;
		scalev.vec.vec = _mm_set_ps1(scaling);
		mat4 scale = mat4::scaling(scalev);
		mat4 rot = mat4::rotationquaternion(rotation);
		vec4 rotc = rotationCenter;
		rotc.set_w(0.0f);
		vec4 trans = translation;
		trans.set_w(0.0f);

		mat4 m = scale;
		m.setrow3(m.getrow3() - rotc);
		m = mat4::multiply(m, rot);
		m.setrow3(m.getrow3() + rotc + trans);
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	mat4
	mat4::rotationquaternion(const quaternion& q)
	{
		float xx = q.x() * q.x();
		float xy = q.x() * q.y();
		float xz = q.x() * q.z();
		float xw = q.x() * q.w();

		float yy = q.y() * q.y();
		float yz = q.y() * q.z();
		float yw = q.y() * q.w();

		float zz = q.z() * q.z();
		float zw = q.z() * q.w();

		float m00 = 1 - 2 * (yy + zz);
		float m01 = 2 * (xy - zw);
		float m02 = 2 * (xz + yw);

		float m10 = 2 * (xy + zw);
		float m11 = 1 - 2 * (xx + zz);
		float m12 = 2 * (yz - xw);

		float m20 = 2 * (xz - yw);
		float m21 = 2 * (yz + xw);
		float m22 = 1 - 2 * (xx + yy);

		float m03 = 0;
		float m13 = 0;
		float m23 = 0;
		float m30 = 0;
		float m31 = 0;
		float m32 = 0;
		float m33 = 1;

		return mat4(vec4(m00, m01, m02, m03),
					vec4(m10, m11, m12, m13),
					vec4(m20, m21, m22, m23),
					vec4(m30, m31, m32, m33));
	}

	//------------------------------------------------------------------------------
	/**
	*/
	mat4
	mat4::transformation(vec4 const &scalingCenter, const quaternion& scalingRotation, vec4 const &scaling, vec4 const &rotationCenter, const quaternion& rotation, vec4 const &translation)
	{
		vec4 scalc = scalingCenter;
		scalc.set_w(0.0f);
		vec4 nscalc = -scalc;
		vec4 rotc = rotationCenter;
		rotc.set_w(0.0f);
		vec4 trans = translation;
		trans.set_w(0.0f);

		mat4 mscaletrans = mat4::translation(nscalc);
		mat4 mscalerotate = mat4::rotationquaternion(scalingRotation);
		mat4 mscalerotateinv = mat4::transpose(mscalerotate);
		mat4 mrotate = mat4::rotationquaternion(rotation);
		mat4 mscale = mat4::scaling(scaling);

		mat4 m = mat4::multiply(mscaletrans, mscalerotateinv);
		m = mat4::multiply(m, mscale);
		m = mat4::multiply(m, mscalerotate);
		m.setrow3(m.getrow3() + scalc - rotc);
		m = mat4::multiply(m, mrotate);
		m.setrow3(m.getrow3() + rotc + trans);
		return m;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
	mat4::ispointinside(const vec4& p, const mat4& m)
	{
		vec4 p1 = mat4::transform(p, m);
		// vectorized compare operation
		return !(vec4::less4_any(vec4(p1.x(), p1.w(), p1.y(), p1.w()),
			vec4(-p1.w(), p1.x(), -p1.w(), p1.y()))
			||
			vec4::less4_any(vec4(p1.z(), p1.w(), 0, 0),
			vec4(-p1.w(), p1.z(), 0, 0)));
	}
} // namespace Math
