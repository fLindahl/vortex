#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include <map>
#include "surfacecollider.h"
#include "physicsserver.h"

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

    void AddRigidBody(Ptr<RigidBody> rBody);
	void RemoveRigidBody(Ptr<RigidBody> rBody);
	
private:
    Util::Array<Ptr<RigidBody>> rigidBodies;

};
}