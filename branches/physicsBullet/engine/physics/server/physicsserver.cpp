#include "config.h"
#include "foundation/math/line.h"
#include "physicsserver.h"
#include "foundation/math/math.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/resourceserver.h"

namespace Physics
{
PhysicsServer::PhysicsServer()
{

}

bool PhysicsServer::Raycast(PhysicsHit& out, const Math::vec4 &position, const Math::vec4 &direction, const float& length)
{
	//Create line from position, in length * direction
	Math::line ray = Math::line(position, direction, length);
	return this->Raycast(out, ray);
}

bool PhysicsServer::Raycast(PhysicsHit& out, const Math::line& ray)
{
    
}

void PhysicsServer::addPhysicsEntity(Game::PhysicsEntity *p)
{
	auto it = this->physicsEntities.Find(p);
	if (it == nullptr)
	{
		this->physicsEntities.Append(p);
	}
	else
	{
		printf("WARNING: PhysicsServer::addPhysicsEntity() >> Entity already added to list!\n");
	}
}

void PhysicsServer::removePhysicsEntity(Game::PhysicsEntity *p)
{
	auto it = this->physicsEntities.Find(p);
	if (it != nullptr)
	{
		//Erase and move last element to this position.
		//Destroys sorting!
		this->physicsEntities.RemoveSwap(it);
	}
}

std::shared_ptr<Physics::BaseCollider> PhysicsServer::LoadCollider(const std::string& meshName, const Physics::ColliderShape& shape)
{
	/*
	if (this->collidersByMeshName.count(meshName) > 0)
	{
		//Mesh has already been cooked and we#ll just return that one.
		return this->collidersByMeshName[meshName];
	}
	else
	{
		//We need to create a physics collider and cook the mesh
		if (shape == Physics::ColliderShape::SURFACE)
		{
			std::shared_ptr<SurfaceCollider> coll = std::make_shared<SurfaceCollider>();
			coll->SetShape(shape);
			coll->CookMeshData(Render::ResourceServer::Instance()->LoadMesh(meshName));
			return coll;
		}
		else
		{
			printf("ERROR: We only support surface colliders so far!");
			assert(false);
		}
		return nullptr;
	}
	*/
}

}