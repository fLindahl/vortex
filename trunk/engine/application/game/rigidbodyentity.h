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
public:
    RigidBodyEntity();
    ~RigidBodyEntity();

	///Registers this entity to the physics server
    void Activate();
    void Deactivate();

    void Update();
    void FixedUpdate();

    void SetTransform(const Math::mat4& t);

    std::shared_ptr<Physics::RigidBody> GetRigidBody() { return this->rigidBody; }
    void SetRigidBody(std::shared_ptr<Physics::RigidBody> r);

private:
    std::shared_ptr<Physics::RigidBody> rigidBody;

};
}	