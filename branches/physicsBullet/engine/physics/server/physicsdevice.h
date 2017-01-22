#pragma once
#include "foundation/util/array.h"
#include "foundation/math/point.h"
#include <map>

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

    void Solve();

	void DebugDraw(int debugDrawFlags);

	void CreateEmptyDynamicsWorld();

	void DestroyDynamicsWorld();

private:
	double timeStep;

	//Bullet members
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface*	m_broadphase;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
};
}