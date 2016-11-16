#pragma once

#include <foundation/util/array.h>
#include "rigidbody.h"
namespace Physics
{

class PhysicsDevice
{
private:
    PhysicsDevice();

public:
    static PhysicsDevice* Instance()
    {
        static PhysicsDevice instance;
        return &instance;
    }

    // C++ 11
    // Delete the methods we don't want.
    PhysicsDevice(PhysicsDevice const&) = delete;
    void operator=(PhysicsDevice const&) = delete;

    void AddRigidBody(std::shared_ptr<RigidBody> rBody);

    void Solve();

private:
    double time;
    double frameTime;

    Util::Array<std::shared_ptr<RigidBody>> rigidBodies;

};

}