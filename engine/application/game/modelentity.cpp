#include "config.h"
#include "modelentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{
	__ImplementClass(Game::ModelEntity, 'mdle', Game::Entity)

ModelEntity::ModelEntity() : Entity()
{
	//Calls baseclass first
	this->gProperty = new Render::GraphicsProperty();
}
	
ModelEntity::~ModelEntity()
{
	delete this->gProperty;
}

void ModelEntity::SetModel(Ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);	
}

void ModelEntity::Activate()
{
	this->gProperty->SetOwner(this);
	this->gProperty->Activate();
	Entity::Activate();
}

void ModelEntity::Deactivate()
{
	this->gProperty->Deactivate();
	Entity::Deactivate();
}

void ModelEntity::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
}

}