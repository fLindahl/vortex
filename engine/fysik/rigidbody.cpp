#include "config.h"
#include "rigidbody.h"
#include "render/resources/modelinstance.h"
#include "render/properties/graphicsproperty.h"

namespace Physics
{

RigidBody::RigidBody() 
{
	this->currentState.orientation = Math::quaternion::identity();
	this->currentState.acceleration = Math::point::zerovector();
	this->currentState.linearVelocity = Math::point::zerovector();
	this->currentState.angularVelocity = Math::point::zerovector();
	this->currentState.force = Math::point::zerovector();
	this->currentState.torque = Math::point::zerovector();
}

RigidBody::~RigidBody()
{

}

void RigidBody::initialize(const float &mass, const Math::mat4 &bodyInertiaTensor, Game::RigidBodyEntity* entity)
{
    this->owner = entity;
    this->collider = this->owner->GetCollider();

    this->massCenter = (this->collider->getbbox().maxPoint + this->collider->getbbox().minPoint) * 0.5f;
	this->currentState.position = entity->GetTransform().get_position() + this->massCenter;
	this->currentState.orientation = Math::mat4::rotationmatrix(entity->GetTransform());

    this->mass = mass;
    this->massInv = 1.0f/this->mass;

    //Set modelspace inertia tensor and inverse tensor.
    this->inertiaTensor = bodyInertiaTensor;
    this->invInertiaTensor = Math::mat4::inverse(this->inertiaTensor);

    this->calculateDerivedQuantities(this->currentState);
}

void RigidBody::setCollider(std::shared_ptr<Physics::SurfaceCollider> coll)
{
    this->collider = coll;
}

void RigidBody::applyForce(const Math::vec4 &dir, const float &magnitude)
{
	this->currentState.force += dir*magnitude;
}

void RigidBody::applyForceAtPoint(const Math::vec4 &dir, const float &magnitude, const Math::point &worldPos)
{
	Math::point impulse = dir*magnitude;
	this->currentState.force += impulse;

    Math::point relativeContactPosition = worldPos - this->currentState.position;
	Math::point impulsiveTorque = Math::point::cross3(impulse, relativeContactPosition);
	this->currentState.torque += Math::mat4::transform(impulsiveTorque, this->invInertiaTensor);

}

BodyState RigidBody::Integrate(const BodyState& oldState, const double& frameTime)
{
	BodyState newState;

	auto Euler = [&](const double& frameTime)->void
	{
		newState = Evaluate(oldState, frameTime, oldState);
	};

	auto MidPoint = [&](const double& frameTime)->void
	{
		BodyState mid = Evaluate(oldState, frameTime * 0.5f, oldState);

		newState = Evaluate(oldState, frameTime, mid);
	};

	auto RK4 = [&](const double& frameTime)->void
	{
		BodyState w1 = oldState;
		BodyState w2 = Evaluate(oldState, frameTime * 0.5f, w1);
		BodyState w3 = Evaluate(oldState, frameTime * 0.5f, w2);
		BodyState w4 = Evaluate(oldState, frameTime, w3);

		newState.linearVelocity = (w1.linearVelocity + (w2.linearVelocity + w3.linearVelocity) * 2.0f + w4.linearVelocity)* (1.0f / 6.0f);
		newState.angularVelocity = (w1.angularVelocity + (w2.angularVelocity + w3.angularVelocity) * 2.0f + w4.angularVelocity) * (1.0f / 6.0f);

		newState = Evaluate(oldState, frameTime, newState);
	};

	RK4(frameTime);

	return newState;

}

BodyState RigidBody::Evaluate(const BodyState& oldState, const double& frameTime, const BodyState& derivative)
{
	BodyState newState;

	newState.dt = frameTime;

	Math::point lastFrameAcceleration = oldState.acceleration;
	lastFrameAcceleration += oldState.force * this->massInv;

	Math::point angularAcceleration = Math::mat4::transform(oldState.torque, oldState.invInertiaTensorWorld);

	newState.linearVelocity = oldState.linearVelocity + lastFrameAcceleration;
	newState.angularVelocity = oldState.angularVelocity + angularAcceleration * frameTime;
	newState.position = oldState.position + (derivative.linearVelocity * frameTime);

	Math::quaternion q = Math::quaternion::multiply(oldState.orientation, Math::quaternion(derivative.angularVelocity.x() * frameTime, derivative.angularVelocity.y() * frameTime, derivative.angularVelocity.z() * frameTime, 0.0f));
	Math::quaternion::scale(q, 0.5f);

	newState.orientation.set(oldState.orientation.x() + q.x(), oldState.orientation.y() + q.y(), oldState.orientation.z() + q.z(), oldState.orientation.w() + q.w());
	newState.orientation = Math::quaternion::normalize(newState.orientation);

	newState.force = Math::vector::zerovector();
	newState.torque = Math::vector::zerovector();

	return newState;
}

void RigidBody::update(const double& frameTime)
{
    assert(frameTime > 0.0);
	
	this->previousState = this->currentState;
	this->currentState = Integrate(this->previousState, frameTime);
	this->calculateDerivedQuantities(currentState);
}

void RigidBody::calculateDerivedQuantities(BodyState& state)
{
    state.R = Math::mat4::rotationquaternion(state.orientation);
    Math::mat4 invBodyTensorRotated = Math::mat4::multiply(this->invInertiaTensor, Math::mat4::transpose(state.R));
    state.invInertiaTensorWorld = Math::mat4::multiply(state.R, invBodyTensorRotated);
    state.transform = state.R;
    state.transform = Math::mat4::multiply(state.transform, Math::mat4::translation(state.position));
}

bool RigidBody::collide()
{
	return false;
}

}