#pragma once

#include "surfacecollider.h"
namespace Physics
{
class RigidBody
{
public:
    RigidBody();
    ~RigidBody();

    void applyForce(const Math::vec4& dir, const float& magnitude);

    void setCollider(std::shared_ptr<SurfaceCollider> coll);


private:
    friend class PhysicsDevice;

    void calculate(const double& frameTime);
    bool collide();

    Math::point position;
    Math::vec4 velocity;
    Math::vec4 angularVelocity;
    Math::quaternion orientation;
    float mass;
    Math::vec4 massCenter;


    std::shared_ptr<SurfaceCollider> collider;
};

}