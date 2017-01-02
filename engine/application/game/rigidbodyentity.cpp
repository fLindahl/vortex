#include "config.h"
#include "fysik/physicsdevice.h"
#include "render/properties/graphicsproperty.h"

namespace Game
{

RigidBodyEntity::RigidBodyEntity()
{
	this->physicsType = Physics::PhysicsType::Rigidbody;
	this->rigidBody = std::make_shared<Physics::RigidBody>();
}

RigidBodyEntity::~RigidBodyEntity()
{
	this->Deactivate();
}

void RigidBodyEntity::Update()
{
    Entity::Update();

    //TODO: This should be a message, sent by the rigidbody
	//TODO: this is ugly.
    this->SetTransform(Math::mat4::multiply(Math::mat4::translation(this->rigidBody->getCenterOfMass() * -1.0f), this->rigidBody->getTransform()));
}

void RigidBodyEntity::SetTransform(const Math::mat4 &t)
{
    Entity::SetTransform(t);
    this->gProperty->setModelMatrix(t);

}

void RigidBodyEntity::Activate()
{
	if (!this->active)
	{
		if (!this->rigidBody->initialized)
		{
			this->rigidBody->initialize(1.0f, Physics::PhysicsServer::CalculateInertiaTensor(this->collider, 1.0f), this);
		}
		Physics::PhysicsDevice::Instance()->AddRigidBody(this->rigidBody);
		PhysicsEntity::Activate();
	}
}

void RigidBodyEntity::Deactivate()
{
	if (this->active)
	{
		Physics::PhysicsDevice::Instance()->RemoveRigidBody(this->rigidBody);
		PhysicsEntity::Deactivate();
	}
}

void RigidBodyEntity::SetRigidBody(std::shared_ptr<Physics::RigidBody> r)
{
    this->rigidBody = r;
}
void RigidBodyEntity::FixedUpdate()
{
    Entity::FixedUpdate();
}

}