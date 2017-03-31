#pragma once
#include "physicsentity.h"

namespace Physics
{
class RigidBody;
}

namespace Game
{
class RigidBodyEntity : public PhysicsEntity
{

	__DeclareClass(RigidBodyEntity)
public:
    RigidBodyEntity();
    ~RigidBodyEntity();

	///Registers this entity to the physics server
    void Activate();
    void Deactivate();

    void Update();
    void FixedUpdate();

    void SetTransform(const Math::mat4& t);

    Ptr<Physics::RigidBody> GetRigidBody() { return this->rigidBody; }
    void SetRigidBody(Ptr<Physics::RigidBody> r);

private:
    Ptr<Physics::RigidBody> rigidBody;

};
}	