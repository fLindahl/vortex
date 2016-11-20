#pragma once
#include "application/game/rigidbodyentity.h"
#include "application/game/baseproperty.h"
#include "surfacecollider.h"

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

	Math::mat4 getTransform() { return this->transform; }
	Math::point getCenterOfMass() { return this->massCenter; }
	Math::quaternion getOrientation() { return this->orientation; }
	Math::point getPosition() { return this->position; }
	Math::vector getLinearVelocity() { return this->linearVelocity; }
	Math::vector getAngularVelocity() { return this->angularVelocity; }
	Math::vector getForce() { return this->force; }
	Math::vector getTorque() { return this->torque; }
	Math::vector getAcceleration() { return this->acceleration; }


private:
    friend class PhysicsDevice;
    friend class Game::RigidBodyEntity;

    void initialize(const float& mass, const Math::mat4& bodyInertiaTensor, Game::RigidBodyEntity* entity);

    void update(const double& frameTime);
    bool collide();

    void calculateDerivedQuantities();

    //Constant quantities
    float mass;
    float massInv;
    Math::point massCenter;
    Math::mat4 inertiaTensor; // model coordiantes
    Math::mat4 invInertiaTensor; // model coordiantes

    //State Variables
    Math::point position; //Cm on worldspace
    Math::vector acceleration;
    Math::quaternion orientation; //q

    //derived quantities
    Math::mat4 R; //orientation matrix
    Math::vec4 linearVelocity; //v
    Math::vec4 angularVelocity; //w
    Math::mat4 transform;
    Math::mat4 invInertiaTensorWorld;

    //accumilative quantities
    Math::vector force;
    Math::vector torque;

    Game::RigidBodyEntity* owner;
    std::shared_ptr<Physics::BaseCollider> collider;
};

}