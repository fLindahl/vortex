#include "config.h"
#include "fysik/physicsdevice.h"
#include "render/properties/graphicsproperty.h"

namespace Game
{
	__ImplementClass(Game::RigidBodyEntity, 'rben', Game::PhysicsEntity)

RigidBodyEntity::RigidBodyEntity()
{
	this->physicsType = Physics::PhysicsType::Rigidbody;
	this->rigidBody = Physics::RigidBody::Create();
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
	PhysicsEntity::SetTransform(t);
}

void RigidBodyEntity::Activate()
{
	if (!this->active)
	{
		if (!this->rigidBody->IsInitialized())
		{
			this->rigidBody->Initialize(1.0f, Physics::PhysicsServer::CalculateInertiaTensor(this->collider.downcast<Physics::BaseCollider>(), 1.0f), this);
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

void RigidBodyEntity::SetRigidBody(Ptr<Physics::RigidBody> r)
{
    this->rigidBody = r;
}
void RigidBodyEntity::FixedUpdate()
{
    Entity::FixedUpdate();
}

}