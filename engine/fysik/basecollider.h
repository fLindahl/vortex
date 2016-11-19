#pragma once
#include "foundation/util/array.h"

namespace Physics
{

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