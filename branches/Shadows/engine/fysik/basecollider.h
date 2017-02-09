#pragma once
#include "foundation/util/array.h"
#include "physicsserver.h"

namespace Physics
{

class BaseCollider
{
public:
    BaseCollider();
    virtual ~BaseCollider();

	const Math::bbox& getbbox() { return this->colliderbbox; }

	void SetShape(const ColliderShape& s) { this->shape = s; }
	ColliderShape GetShape() { return this->shape; }

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