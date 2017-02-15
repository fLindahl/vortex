#include "config.h"
#include "boxentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{

BoxEntity::BoxEntity()
{
	//Call baseclass first!
	Entity();
	this->gProperty = new Render::GraphicsProperty();
}

BoxEntity::~BoxEntity()
{
	delete this->gProperty;
}

void BoxEntity::SetModel(std::shared_ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);
}

void BoxEntity::Activate()
{
	this->gProperty->SetOwner(this->shared_from_this());
	this->gProperty->Activate();
	Entity::Activate();
}

void BoxEntity::Deactivate()
{
	this->gProperty->Deactivate();
	Entity::Deactivate();
}

void BoxEntity::Update()
{
	Entity::Update();
	this->SetTransform(Entity::GetTransform());
}

void BoxEntity::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
}

}