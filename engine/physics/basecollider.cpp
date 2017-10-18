#include "config.h"
#include "basecollider.h"
#include "btBulletDynamicsCommon.h"


namespace Physics
{
__ImplementClass(Physics::BaseCollider, 'bcol', Core::RefCounted)
	
BaseCollider::BaseCollider()
{

}

BaseCollider::~BaseCollider()
{

}

btCollisionShape* BaseCollider::GetBtCollisionShape() const
{
	return this->btCollider;
}

void BaseCollider::SetShape(const ColliderShape& s)
{ 
	this->shape = s; 
}

ColliderShape BaseCollider::GetShape() 
{ 
	return this->shape;
}


}