#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include <map>
#include "surfacecollider.h"
#include "physicsserver.h"

#include "btBulletDynamicsCommon.h"


namespace Physics
{

class RigidBody;

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
	
	btDefaultCollisionConfiguration* collisionConfiguration;

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher;

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache;

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver;

	btDiscreteDynamicsWorld* dynamicsWorld;


private:
    Util::Array<Ptr<RigidBody>> rigidBodies;

	

};
}