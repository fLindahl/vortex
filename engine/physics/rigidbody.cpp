#include "config.h"
#include "rigidbody.h"
#include "render/resources/modelinstance.h"
#include "render/properties/graphicsproperty.h"
#include "btBulletDynamicsCommon.h"
#include "bulletvortexconversion.h"
#include "physicsdevice.h"

namespace Physics
{

__ImplementClass(Physics::RigidBody, 'RGBD', Core::RefCounted)
RigidBody::RigidBody() :
	initialized(false),
	body(nullptr),
	registered(false),
	mass(1.0f)
{

}

RigidBody::~RigidBody()
{
	if (initialized)
	{
		this->Uninitialize();
	}
}

bool RigidBody::Initialize()
{
	if (this->initialized)
		return true;

	if (!this->collider.isvalid())
	{
		_error("Could not initialize Rigidbody since no collider has been set!");
		return false;
	}

	_assert2(this->body == nullptr, "btRigidBody has been instantiated but not initialized! Make sure you aren't creating a btRigidBody by not calling Initialize!");

	btTransform startTransform;
	startTransform.setIdentity();

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (this->mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		this->collider->GetBtCollisionShape()->calculateLocalInertia(mass, localInertia);

	//startTransform.setOrigin(btVector3(2, 10, 0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	this->motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, this->collider->GetBtCollisionShape(), localInertia);
	
	this->body = new btRigidBody(rbInfo);

	this->initialized = true;

	return true;
}

bool RigidBody::Uninitialize()
{
	if (!this->initialized)
		return true;

	if(this->registered)
	{
		PhysicsDevice::Instance()->RemoveRigidBody(this);
	}

	delete this->motionState;
	delete this->body;

	this->body = nullptr;

	this->initialized = false;

	return true;
}

void RigidBody::SetMass(float m)
{
	_assert(m < 0.0f);

	this->mass = m;	
	this->ReloadBtRigidBody();
}

float RigidBody::GetMass() const
{
	return this->mass;
}

void RigidBody::SetCollider(const Ptr<BaseCollider>& collider)
{
	this->collider = collider;
	
	//Make sure to re-register if necessary
	bool reg = this->registered;

	if (initialized)
	{
		this->Uninitialize();
		this->Initialize();
	}

	if (reg)
	{
		PhysicsDevice::Instance()->AddRigidBody(this);
	}
}

Math::mat4 RigidBody::GetStateTransform() const
{
	return Bt2VortexTransform(this->body->getWorldTransform());
}

void RigidBody::SetStateTransform(const Math::mat4 & m)
{
	this->body->setWorldTransform(Vortex2BtTransform(m));
}

Ptr<BaseCollider> RigidBody::GetCollider() const
{
	return this->collider;
}

void RigidBody::ApplyForce(const Math::vec4& dir, const float& magnitude)
{
	btVector3 f = Vortex2BtVector3(dir * magnitude);
	this->body->applyCentralForce(f);
}

void RigidBody::ApplyForceAtPoint(const Math::vec4& dir, const float& magnitude, const Math::point& worldPos)
{
	btVector3 f = Vortex2BtVector3(dir * magnitude);
	btVector3 p = Vortex2BtVector3(worldPos);
	this->body->applyForce(f, p);
}

void RigidBody::ReloadBtRigidBody()
{
	if (this->initialized)
	{
		if (this->registered)
		{
			//Remove the rigid body from the dynamics world
			PhysicsDevice::Instance()->dynamicsWorld->removeRigidBody(this->body);
			btVector3 inertia;
			this->body->getCollisionShape()->calculateLocalInertia(mass, inertia);
			this->body->setMassProps(mass, inertia);
			//Add the rigid body to the dynamics world
			PhysicsDevice::Instance()->dynamicsWorld->addRigidBody(this->body);
		}
		else
		{
			btVector3 inertia;
			this->body->getCollisionShape()->calculateLocalInertia(mass, inertia);
			this->body->setMassProps(mass, inertia);
		}
	}
}


}