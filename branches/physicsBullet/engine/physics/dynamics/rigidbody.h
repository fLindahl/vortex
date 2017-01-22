#pragma once
#include "application/game/rigidbodyentity.h"
#include "application/game/baseproperty.h"
#include "physics/dynamics/rigidbody.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletDynamics/Dynamics/btRigidBody.h"

namespace Render
{
	class GraphicsProperty;
}

namespace Physics
{

class RigidBody
{
public:
    RigidBody();
    ~RigidBody();

    void applyForce(const Math::vec4& dir, const float& magnitude);
    void applyForceAtPoint(const Math::vec4& dir, const float& magnitude, const Math::point& worldPos);

    //void setGraphicsProperty(Render::GraphicsProperty* gp) {this->gProperty = gp;}
    void setCollider(std::shared_ptr<Physics::SurfaceCollider> coll);

	Math::mat4 GetTransform() { return this->body->getCenterOfMassTransform(); }
	Math::quaternion GetOrientation() { return this->currentState.orientation; }
	Math::point GetCenterOfMass() { return this->massCenter; }
	Math::point GetPosition() { return this->currentState.position; }
	Math::point GetLinearVelocity() { return this->currentState.linearVelocity; }
	Math::point GetAngularVelocity() { return this->currentState.angularVelocity; }
	Math::point GetForce() { return this->currentState.force; }
	Math::point GetTorque() { return this->currentState.torque; }
	Math::point GetAcceleration() { return this->currentState.acceleration; }

private:
    friend class PhysicsDevice;
    friend class Game::RigidBodyEntity;

	bool initialized;
    void initialize(const float& mass, const Math::mat4& bodyInertiaTensor, Game::RigidBodyEntity* entity);

    void update(const double& frameTime);
    bool collide();

	btRigidBody* body;

    Game::RigidBodyEntity* owner;
    std::shared_ptr<Physics::BaseCollider> collider;
};

}