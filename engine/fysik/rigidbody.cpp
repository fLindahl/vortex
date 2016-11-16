#include "config.h"
#include "rigidbody.h"

namespace Physics
{

RigidBody::RigidBody()
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::setCollider(std::shared_ptr<SurfaceCollider> coll)
{
    this->collider = coll;

    Math::vec4 c1 = this->collider->getbbox().corner_point(0);
    Math::vec4 c2 = this->collider->getbbox().corner_point(1);
    Math::vec4 c3 = this->collider->getbbox().corner_point(2);
    Math::vec4 c4 = this->collider->getbbox().corner_point(3);
    Math::vec4 c5 = this->collider->getbbox().corner_point(4);
    Math::vec4 c6 = this->collider->getbbox().corner_point(5);
    Math::vec4 c7 = this->collider->getbbox().corner_point(6);
    Math::vec4 c8 = this->collider->getbbox().corner_point(7);

    this->massCenter = (c1+c2+c3+c4+c5+c6+c7+c8) * 0.125f;

    
}

void RigidBody::applyForce(const Math::vec4 &dir, const float &magnitude)
{
    //TODO: Lets apply velocity later. Start with spinvector!
    //this->velocity = this->velocity + ((dir * magnitude) * (1.0f/this->mass));
}

void RigidBody::calculate(const double& frameTime)
{
    this->position += velocity * frameTime;
}

bool RigidBody::collide()
{

}

}