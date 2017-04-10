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
	this->gProperty = Render::GraphicsProperty::Create();
	this->AddProperty(this->gProperty.downcast<Game::BaseProperty>());
}
	
ModelEntity::~ModelEntity()
{
	
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

}