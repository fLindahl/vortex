#include "config.h"
#include "modelentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{

ModelEntity::ModelEntity()
{
	//Call baseclass first!
	Entity::Entity();
	this->gProperty = new Render::GraphicsProperty();
}
	
ModelEntity::~ModelEntity()
{
	delete this->gProperty;
}

void ModelEntity::SetModel(std::shared_ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);
	
}

void ModelEntity::Activate()
{
	this->gProperty->SetOwner(this->shared_from_this());
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