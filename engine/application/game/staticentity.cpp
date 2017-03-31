#include "config.h"
#include "staticentity.h"
#include "render/properties/graphicsproperty.h"

namespace Game
{
	__ImplementClass(Game::StaticEntity, 'sten', Game::PhysicsEntity)
StaticEntity::StaticEntity()
{
	this->physicsType = Physics::PhysicsType::Static;
}

StaticEntity::~StaticEntity()
{

}

void StaticEntity::Activate()
{
    PhysicsEntity::Activate();
}
void StaticEntity::Deactivate()
{
	PhysicsEntity::Deactivate();
}
void StaticEntity::SetTransform(const Math::mat4 &t)
{
    Entity::SetTransform(t);
    this->gProperty->setModelMatrix(t);
}

}