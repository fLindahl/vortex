#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include "foundation/math/bbox.h"

#include "btBulletDynamicsCommon.h"

namespace Physics
{

enum ColliderShape
{
	EMPTY,
	BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	CONE,
	MULTISPHERE,
	CONVEXHULL,
	CONVEXTRIANGLEMESH,
	BVHTRIANGLEMESH,
	HEIGHTFIELDTERRAIN,
	STATICPLANE,
	COMPOUND
};

class BaseCollider : public Core::RefCounted
{
	__DeclareClass(BaseCollider)
public:
	BaseCollider();
	virtual ~BaseCollider();


	///Returns the bullet collision shape.
	///@note	Unsafe and should be used with caution. Never delete pointer content or change it from anywhere but here.
	btCollisionShape* GetBtCollisionShape() const;

	void SetShape(const ColliderShape& s);
	ColliderShape GetShape();

protected:
	ColliderShape shape;

	btCollisionShape* btCollider;
};

}