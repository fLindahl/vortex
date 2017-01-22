#include "config.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <foundation/math/math.h>
#include "physicsdevice.h"
#include <list>
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace Physics
{

PhysicsDevice::PhysicsDevice() :
	timeStep(0.02)
{
}

void PhysicsDevice::Solve()
{
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(this->timeStep);
	}
}

void PhysicsDevice::CreateEmptyDynamicsWorld()
{
	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void PhysicsDevice::DestroyDynamicsWorld()
{
	//removePickingConstraint();
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them

	if (m_dynamicsWorld)
	{

		int i;
		for (i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
		{
			m_dynamicsWorld->removeConstraint(m_dynamicsWorld->getConstraint(i));
		}
		for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
	}
	//delete collision shapes
	for (int j = 0; j<m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	m_dynamicsWorld = 0;

	delete m_solver;
	m_solver = 0;

	delete m_broadphase;
	m_broadphase = 0;

	delete m_dispatcher;
	m_dispatcher = 0;

	delete m_collisionConfiguration;
	m_collisionConfiguration = 0;
}

void PhysicsDevice::DebugDraw(int debugDrawFlags)
{
	if (m_dynamicsWorld)
	{
		if (m_dynamicsWorld->getDebugDrawer())
		{
			m_dynamicsWorld->getDebugDrawer()->setDebugMode(debugDrawFlags);
		}
		m_dynamicsWorld->debugDrawWorld();
	}

}

}