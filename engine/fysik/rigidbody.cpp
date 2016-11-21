#include "config.h"
#include "rigidbody.h"
#include "render/resources/modelinstance.h"
#include "render/properties/graphicsproperty.h"

namespace Physics
{

RigidBody::RigidBody() :
 acceleration(0.0f),
 linearVelocity(0.0f),
 angularVelocity(0.0f),
 force(0.0f),
 torque(0.0f)
{
	this->orientation = Math::quaternion::identity();
}

RigidBody::~RigidBody()
{

}

void RigidBody::initialize(const float &mass, const Math::mat4 &bodyInertiaTensor, Game::RigidBodyEntity* entity)
{
    this->owner = entity;
    this->collider = this->owner->GetCollider();

    this->massCenter = (this->collider->getbbox().maxPoint + this->collider->getbbox().minPoint) * 0.5f;
    this->position = entity->GetTransform().get_position() + this->massCenter;
    this->orientation = Math::mat4::rotationmatrix(entity->GetTransform());

    this->mass = mass;
    this->massInv = 1.0f/this->mass;

    //Set modelspace inertia tensor and inverse tensor.
    this->inertiaTensor = bodyInertiaTensor;
    this->invInertiaTensor = Math::mat4::inverse(this->inertiaTensor);

    this->calculateDerivedQuantities();
}

void RigidBody::setCollider(std::shared_ptr<Physics::SurfaceCollider> coll)
{
    this->collider = coll;
}

void RigidBody::applyForce(const Math::vec4 &dir, const float &magnitude)
{
    this->force += dir*magnitude;
}

void RigidBody::applyForceAtPoint(const Math::vec4 &dir, const float &magnitude, const Math::point &worldPos)
{
    Math::vector impulse = dir*magnitude;
    this->force += impulse;

    Math::point relativeContactPosition = worldPos - this->position;
    Math::vector impulsiveTorque = Math::vector::cross3(impulse, relativeContactPosition);
    this->torque += Math::mat4::transform(impulsiveTorque, this->invInertiaTensor);
}

void RigidBody::update(const double& frameTime)
{
    assert(frameTime > 0.0);
	
    Math::vector lastFrameAcceleration = this->acceleration;
    lastFrameAcceleration += force * massInv;

	this->linearVelocity += lastFrameAcceleration;

	//this->position += (this->linearVelocity * frameTime);

    Math::vector angularAcceleration = Math::mat4::transform(torque, invInertiaTensorWorld);

    this->angularVelocity += angularAcceleration * frameTime;

	Math::quaternion q = Math::quaternion::multiply(this->orientation, Math::quaternion(this->angularVelocity.x() * frameTime, this->angularVelocity.y() * frameTime, this->angularVelocity.z() * frameTime, 0.0f));
	Math::quaternion::scale(q, 0.5f);

	this->orientation.set(this->orientation.x() + q.x(), this->orientation.y() + q.y(), this->orientation.z() + q.z(), this->orientation.w() + q.w());
    this->orientation = Math::quaternion::normalize(this->orientation);

    this->calculateDerivedQuantities();

    this->force = Math::vector::zerovector();
    this->torque = Math::vector::zerovector();
}

void RigidBody::calculateDerivedQuantities()
{
    this->R = Math::mat4::rotationquaternion(this->orientation);
    //this->R = Math::mat4::multiply(this->R, Math::mat4::translation(this->massCenter));
    Math::mat4 invBodyTensorRotated = Math::mat4::multiply(this->invInertiaTensor, Math::mat4::transpose(this->R));
    this->invInertiaTensorWorld = Math::mat4::multiply(this->R, invBodyTensorRotated);
    this->transform = R;
    this->transform = Math::mat4::multiply(this->transform, Math::mat4::translation(this->position));
    //this->transform.set_position(this->position);
}

bool RigidBody::collide()
{
	return false;
}

}