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
	isStatic(false),
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

bool RigidBody::Initialize(const Math::mat4& startTransform)
{
	if (this->initialized)
		return true;

	if (!this->collider.isvalid())
	{
		_error("Could not initialize Rigidbody since no collider has been set!");
		return false;
	}

	_assert2(this->body == nullptr, "btRigidBody has been instantiated but not initialized! Make sure you aren't creating a btRigidBody by not calling Initialize!");
	
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool staticBody = (this->mass == 0.f) || this->isStatic;

	float m = this->mass;

	btVector3 localInertia(0, 0, 0);
	if (!staticBody)
	{
		this->collider->GetBtCollisionShape()->calculateLocalInertia(mass, localInertia);
	}
	else
	{
		m = 0.0f;
	}

	//Remove the scale since this is handled by local scale in bullet
	Math::mat4 t = startTransform;
	Math::vec4 scale;
	t.get_scale(scale);
	t.scale(Math::vec4(1 / scale.x(), 1 / scale.y(), 1 / scale.z(), 1));
	
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	this->motionState = new btDefaultMotionState(Vortex2BtTransform(t));
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m, motionState, this->collider->GetBtCollisionShape(), localInertia);
	
	this->body = new btRigidBody(rbInfo);

	this->initialized = true;

	this->collider->btCollObject = this->body;

	Physics::PhysicsDevice::Instance()->AddRigidBody(this);

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
	_assert(m >= 0.0f);

	this->mass = m;	
	this->ReloadBtRigidBody();
}

bool RigidBody::IsStatic() const
{
	return this->isStatic;
}

void RigidBody::SetStatic(bool val)
{
	this->isStatic = val;
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
		Math::mat4 t = Bt2VortexTransform(this->body->getWorldTransform());
		this->Uninitialize();
		this->Initialize(t);
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