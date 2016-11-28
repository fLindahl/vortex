#include "config.h"
#include "fysik/physicsdevice.h"
#include "render/properties/graphicsproperty.h"

namespace Game
{

RigidBodyEntity::RigidBodyEntity()
{

}

RigidBodyEntity::~RigidBodyEntity()
{

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
    Entity::Activate();

    this->rigidBody->initialize(1.0f, Physics::PhysicsServer::CalculateInertiaTensor(this->collider, 1.0f), this);
    Physics::PhysicsServer::Instance()->addPhysicsEntity(this);

    Physics::PhysicsDevice::Instance()->AddRigidBody(this->rigidBody);

}

void RigidBodyEntity::Deactivate()
{
    Entity::Deactivate();
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