#include "config.h"
#include "staticentity.h"
#include "render/properties/graphicsproperty.h"

namespace Game
{
StaticEntity::StaticEntity()
{
	this->physicsType = Physics::PhysicsType::Static;
}

StaticEntity::~StaticEntity()
{

}

void StaticEntity::Activate()
{
    Game::Entity::Activate();

    Physics::PhysicsServer::Instance()->addPhysicsEntity(this);
}
void StaticEntity::Deactivate()
{
    Entity::Deactivate();
}
void StaticEntity::SetTransform(const Math::mat4 &t)
{
    Entity::SetTransform(t);
    this->gProperty->setModelMatrix(t);
}

}