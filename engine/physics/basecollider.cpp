#include "config.h"
#include "basecollider.h"
#include "btBulletDynamicsCommon.h"
#include "physicsdevice.h"
#include "bulletvortexconversion.h"


namespace Physics
{
__ImplementClass(Physics::BaseCollider, 'bcol', Core::RefCounted)
	
BaseCollider::BaseCollider() :
	initializedStatic(false)
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

void BaseCollider::InitializeStaticObject(const Math::mat4& startTransform)
{
	if (this->initializedStatic)
	{
		return;
	}

	//Remove the scale since this is handled by local scale in bullet
	Math::mat4 t = startTransform;
	Math::vec4 scale;
	t.get_scale(scale);
	t.scale(Math::vec4(1 / scale.x(), 1 / scale.y(), 1 / scale.z(), 1));

	this->btCollObject = new btCollisionObject();

	this->btCollObject->setCollisionShape(this->btCollider);
	this->btCollObject->setWorldTransform(Vortex2BtTransform(t));
	
	Physics::PhysicsDevice::Instance()->AddStaticObject(this);

	this->initializedStatic = true;
}

void BaseCollider::UninitializeStaticObject()
{
	if (!this->initializedStatic)
	{
		return;
	}

	Physics::PhysicsDevice::Instance()->RemoveStaticObject(this);
	delete this->btCollObject;


	this->initializedStatic = false;
}

//void BaseCollider::SetBTUserPointer(const Ptr<Property::Collider>& p)
//{
//	this->btCollider->setUserPointer(p.get_unsafe());
//	
//	if (this->initializedStatic)
//		this->btCollObject->setUserPointer(p.get_unsafe());
//
//}


}