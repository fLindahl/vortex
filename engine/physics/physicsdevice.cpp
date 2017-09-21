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


namespace Physics
{

PhysicsDevice::PhysicsDevice()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->overlappingPairCache = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->overlappingPairCache, this->solver, this->collisionConfiguration);
}

void PhysicsDevice::AddRigidBody(Ptr<RigidBody> rBody)
{
	auto it = this->rigidBodies.Find(rBody);
	if (it == nullptr)
	{
		this->rigidBodies.Append(rBody);
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
	}
}

}