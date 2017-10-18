#include "config.h"
#include "boxcollider.h"
#include "bulletvortexconversion.h"

namespace Physics
{

__ImplementClass(Physics::BoxCollider, 'BCOL', Physics::BaseCollider);

BoxCollider::BoxCollider()
{
	//Note: this is set to a hard value and we just change the local scale if the size is changed. Otherwise we'll need to reinitialize all rigidbodies that use this collider.
	this->btCollider = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
}

BoxCollider::~BoxCollider()
{
	delete this->btCollider;
}

void BoxCollider::SetSize(const Math::vector & localSize)
{
	this->size = localSize;

	this->btCollider->setLocalScaling(Vortex2BtVector3(this->size));
}

Math::vector BoxCollider::GetSize() const
{
	return Math::vector();
}

}