#include "config.h"
#include "physicsentity.h"
#include "render/properties/graphicsproperty.h"
#include "fysik/physicsserver.h"
#include "render/resources/modelinstance.h"

namespace Game
{

PhysicsEntity::PhysicsEntity()
{
	this->gProperty = new Render::GraphicsProperty();
}
	
PhysicsEntity::~PhysicsEntity()
{
	delete this->gProperty;
}

void PhysicsEntity::SetModel(std::shared_ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);
	this->collider = std::dynamic_pointer_cast<Physics::SurfaceCollider>(Physics::PhysicsServer::Instance()->LoadCollider(mdl->GetMesh()->GetName(), Physics::ColliderShape::SURFACE));

}

void PhysicsEntity::SetTransform(const Math::mat4& t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
}

void PhysicsEntity::Activate()
{
	Physics::PhysicsServer::Instance()->addPhysicsEntity(this);
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