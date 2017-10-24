#include "config.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <foundation/util/fixedarray.h>
#include <foundation/math/math.h>
#include "physicsdevice.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/resources/meshresource.h"
#include <list>
#include "colliderproperty.h"
#include "rigidbodyproperty.h"
#include "rigidbody.h"
#include "application/game/baseproperty.h"
#include "btBulletDynamicsCommon.h"

namespace Physics
{

PhysicsDevice::PhysicsDevice()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->overlappingPairCache = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->overlappingPairCache, this->solver, this->collisionConfiguration);
	this->dynamicsWorld->setGravity(btVector3(0, -1.0f, 0));
}

void PhysicsDevice::StepSimulation(float dt)
{
	dynamicsWorld->stepSimulation(dt, 10);
}

void PhysicsDevice::AddRigidBody(Ptr<RigidBody> rBody)
{
	auto it = this->rigidBodies.Find(rBody);
	if (it == nullptr)
	{
		this->rigidBodies.Append(rBody);
		dynamicsWorld->addRigidBody(rBody->body);
		rBody->registered = true;
	}
	else
	{
		printf("WARNING: Rigidbody already added to PhysicsDevice!\n");
	}
}

void PhysicsDevice::RemoveRigidBody(Ptr<RigidBody> rBody)
{
	auto it = this->rigidBodies.Find(rBody);
	if (it != nullptr)
	{
		//Erase and move last element to this position.
		//Destroys sorting!
		this->rigidBodies.RemoveSwap(it);
		dynamicsWorld->removeRigidBody(rBody->body);
		rBody->registered = false;
	}
}

void PhysicsDevice::AddStaticObject(Ptr<BaseCollider> collider)
{
	auto it = this->staticObjects.Find(collider);
	if (it == nullptr)
	{
		this->staticObjects.Append(collider);
		dynamicsWorld->addCollisionObject(collider->btCollObject, short(btBroadphaseProxy::StaticFilter));
	}
	else
	{
		printf("WARNING: Rigidbody already added to PhysicsDevice!\n");
	}
}

void PhysicsDevice::RemoveStaticObject(Ptr<BaseCollider> collider)
{
	auto it = this->staticObjects.Find(collider);
	if (it != nullptr)
	{
		//Erase and move last element to this position.
		//Destroys sorting!
		this->staticObjects.RemoveSwap(it);
		dynamicsWorld->removeCollisionObject(collider->btCollObject);
		
	}
}

}