#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include "foundation/math/bbox.h"

class btCollisionObject;
class btCollisionShape;

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

	void InitializeStaticObject(const Math::mat4& startTransform);
	void UninitializeStaticObject();

protected:
	friend class PhysicsDevice;
	friend class RigidBody;

	//void SetBTUserPointer(const Ptr<Property::Collider>& p);

	bool initializedStatic;

	ColliderShape shape;

	btCollisionShape* btCollider;
	btCollisionObject* btCollObject;
};

}