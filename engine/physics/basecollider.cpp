#include "config.h"
#include "basecollider.h"

namespace Physics
{
__ImplementClass(Physics::BaseCollider, 'bcol', Core::RefCounted)
	
void BaseCollider::SetShape(const ColliderShape& s) 
{ 
	this->shape = s; 
}


ColliderShape BaseCollider::GetShape() 
{ 
	return this->shape; 
}

}