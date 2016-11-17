#include "config.h"
#include "rigidbody.h"
#include "render/resources/modelinstance.h"


namespace Physics
{

RigidBody::RigidBody() :
 acceleration(0.0f)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::initialize(const float &mass, const Math::mat4 &bodyInertiaTensor)
{
    this->collider = this->gProperty->getCollider();

    // Set position to be masscenter (center of bbox)
    this->massCenter = (this->collider->getbbox().maxPoint - this->collider->getbbox().minPoint) * 0.5f;

    this->position = this->gProperty->getModelMatrix().get_position() + this->massCenter;
    this->orientation = Math::quaternion::identity();
    this->linearMomentum = Math::vector::zerovector();
    this->angularMomentum = Math::vector::zerovector();

    this->mass = mass;
    this->massInv = 1.0f/this->mass;

    //Set modelspace inertia tensor and inverse tensor.
    this->inertiaTensor = bodyInertiaTensor;
    this->invInertiaTensor = Math::mat4::inverse(this->inertiaTensor);

    this->calculateDerivedQuantities();
}

void RigidBody::setCollider(std::shared_ptr<SurfaceCollider> coll)
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

    //this->position += (this->linearVelocity * frameTime) + ((this->acceleration + (force * massInv)) * frameTime);

    Math::vector lastFrameAcceleration = this->acceleration;
    lastFrameAcceleration += force * massInv;

    Math::vector angularAcceleration = Math::mat4::transform(torque, invInertiaTensorWorld);

    this->angularVelocity += angularAcceleration * frameTime;

    this->position += (this->linearVelocity * frameTime);

    Math::vector scaledAngVel = this->angularVelocity * frameTime;

    this->orientation = Math::quaternion::multiply(this->orientation, Math::quaternion(0, scaledAngVel.x(), scaledAngVel.y(), scaledAngVel.z()));
    this->orientation = Math::quaternion::scale(this->orientation, 0.5f);

    this->calculateDerivedQuantities();

    this->force = Math::vector::zerovector();
    this->torque = Math::vector::zerovector();
}

bool RigidBody::collide()
{

}

void RigidBody::calculateDerivedQuantities()
{
    this->R = Math::mat4::rotationquaternion(this->orientation);
    Math::mat4 invBodyTensorRotated = Math::mat4::multiply(this->invInertiaTensor, Math::mat4::transpose(this->R));
    this->invInertiaTensorWorld = Math::mat4::multiply(this->R, invBodyTensorRotated);
    this->transform = R;
    this->transform.set_position(this->position);

}


}