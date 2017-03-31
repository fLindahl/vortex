#include "config.h"
#include "physicsentity.h"
#include "render/properties/graphicsproperty.h"
#include "fysik/physicsserver.h"
#include "render/resources/modelinstance.h"

namespace Game
{
	__ImplementClass(Game::PhysicsEntity, 'phen', Game::Entity)
PhysicsEntity::PhysicsEntity()
{
	this->gProperty = new Render::GraphicsProperty();
}
	
PhysicsEntity::~PhysicsEntity()
{
	delete this->gProperty;
}

Ptr<Physics::SurfaceCollider> PhysicsEntity::GetCollider()
{
	return this->collider;
}

void PhysicsEntity::SetModel(Ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);
	this->collider = Physics::PhysicsServer::Instance()->LoadCollider(mdl->GetMesh()->GetName(), Physics::ColliderShape::SURFACE).downcast<Physics::SurfaceCollider>();
}

void PhysicsEntity::SetTransform(const Math::mat4& t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
}

void PhysicsEntity::Activate()
{
	Physics::PhysicsServer::Instance()->addPhysicsEntity(this);
	this->gProperty->SetOwner(this);
	this->gProperty->Activate();
	Entity::Activate();
}

void PhysicsEntity::Deactivate()
{
	Physics::PhysicsServer::Instance()->removePhysicsEntity(this);
	this->gProperty->Deactivate();
	Entity::Deactivate();
}

}