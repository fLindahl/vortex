#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include "foundation/math/bbox.h"

namespace Physics
{

enum ColliderShape
{
	SPHERE,
	CAPSULE,
	BOX,
	SURFACE
};

class BaseCollider : public Core::RefCounted
{
__DeclareClass(BaseCollider);
public:
	BaseCollider();
	virtual ~BaseCollider();

	const Math::bbox& getbbox() { return this->colliderbbox; }

	void SetShape(const ColliderShape& s);
	ColliderShape GetShape();

protected:
	Math::bbox colliderbbox;
	ColliderShape shape;
};

inline BaseCollider::BaseCollider()
{

}

inline BaseCollider::~BaseCollider()
{

}

}