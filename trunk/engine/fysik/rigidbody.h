#pragma once

#include "surfacecollider.h"
namespace Physics
{
class RigidBody
{
public:
    RigidBody();
    ~RigidBody();

private:
    std::shared_ptr<SurfaceCollider> collider;

};

}