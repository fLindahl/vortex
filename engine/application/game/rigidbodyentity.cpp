#include "rigidbodyentity.h"

namespace Game
{


void RigidBodyEntity::Update()
{
    Entity::Update();

    //TODO: This should be a message, sent by the rigidbody
    this->SetTransform(this->rigidBody->getTransform());
}
void RigidBodyEntity::SetTransform(const Math::mat4 &t)
{
    Entity::SetTransform(t);
    this->gProperty->setModelMatrix(t);

}
void RigidBodyEntity::Activate()
{
    Entity::Activate();

}
void RigidBodyEntity::Deactivate()
{
    Entity::Deactivate();
}

}