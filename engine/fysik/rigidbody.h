#pragma once
#include "core/refcounted.h"
#include "application/game/rigidbodyentity.h"
#include "fysik/rigidbodyproperty.h"
#include "application/game/baseproperty.h"
#include "surfacecollider.h"

namespace Render
{
	class GraphicsProperty;
}

namespace Physics
{
struct BodyState
{
    double dt;
	Math::point position; //Cm on worldspace
	Math::point acceleration;
	Math::quaternion orientation; //q
	Math::point linearVelocity; //v
	Math::point angularVelocity; //w
	Math::point force; //Force applied this frame
	Math::point torque; //Torque applied this frame
	Math::mat4 R; //orientation matrix
	Math::mat4 transform;
	Math::mat4 invInertiaTensorWorld;
};

class RigidBody : public Core::RefCounted
{
__DeclareClass(RigidBody)
public:
    RigidBody();
    ~RigidBody();

    void applyForce(const Math::vec4& dir, const float& magnitude);
    void applyForceAtPoint(const Math::vec4& dir, const float& magnitude, const Math::point& worldPos);

    //void setGraphicsProperty(Render::GraphicsProperty* gp) {this->gProperty = gp;}
    void setCollider(Ptr<Physics::SurfaceCollider> coll);

	Math::mat4 getTransform() { return this->currentState.transform; }
	Math::point getCenterOfMass() { return this->massCenter; }
	Math::quaternion getOrientation() { return this->currentState.orientation; }
	Math::point getPosition() { return this->currentState.position; }
	Math::point getLinearVelocity() { return this->currentState.linearVelocity; }
	Math::point getAngularVelocity() { return this->currentState.angularVelocity; }
	Math::point getForce() { return this->currentState.force; }
	Math::point getTorque() { return this->currentState.torque; }
	Math::point getAcceleration() { return this->currentState.acceleration; }
	BodyState& GetCurrentState() { return this->currentState; }
	BodyState& GetPreviousState() { return this->previousState; }

	BodyState Integrate(const BodyState& state, const double& frameTime);
	BodyState Evaluate(const BodyState& state, const double& frameTime, const BodyState& derivative);

	bool IsInitialized() const { return this->initialized; }
	void Initialize(const float& mass, const Math::mat4& bodyInertiaTensor, Game::RigidBodyEntity* entity);

private:
    friend class PhysicsDevice;

	bool initialized;
    

    void update(const double& frameTime);
    bool collide();

    void calculateDerivedQuantities(BodyState& state);

    //Constant quantities
    float mass;
    float massInv;
    Math::point massCenter;
    Math::mat4 inertiaTensor; // model coordiantes
    Math::mat4 invInertiaTensor; // model coordiantes

    //State Variables
	BodyState previousState;
	BodyState currentState;

    //Math::point position; //Cm on worldspace
    //Math::vector acceleration;
    //Math::quaternion orientation; //q
    //Math::vec4 linearVelocity; //v
    //Math::vec4 angularVelocity; //w

    //derived quantities
    //Math::mat4 R; //orientation matrix
    //Math::mat4 transform;
    //Math::mat4 invInertiaTensorWorld;

    //accumilative quantities
    //Math::vector force;
    //Math::vector torque;

    Game::RigidBodyEntity* owner;
    Ptr<Physics::BaseCollider> collider;
};

}