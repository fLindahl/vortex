#pragma once
#include "foundation/util/array.h"

namespace Physics
{

enum ColliderShape
{
	SPHERE,
	CAPSULE,
	BOX,
	SURFACE
};

class BaseCollider
{
public:
    BaseCollider();
    virtual ~BaseCollider();

	
	
private:
	ColliderShape shape;
};

inline BaseCollider::BaseCollider()
{

}

inline BaseCollider::~BaseCollider()
{

}

}