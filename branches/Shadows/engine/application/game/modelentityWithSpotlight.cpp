#include "config.h"
#include "modelentityWithSpotlight.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{

ModelEntitySpotLight::ModelEntitySpotLight()
{
	//Call baseclass first!
	Entity();
	this->gProperty = new Render::GraphicsProperty();
    this->lightEntity = new Render::LightServer::SpotLight();
}
	
ModelEntitySpotLight::~ModelEntitySpotLight()
{
	delete this->gProperty;
    delete this->lightEntity;
}

void ModelEntitySpotLight::SetModel(std::shared_ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);
}

void ModelEntitySpotLight::Activate()
{
	this->gProperty->SetOwner(this->shared_from_this());
	this->gProperty->Activate();
	Entity::Activate();
}

void ModelEntitySpotLight::Deactivate()
{
	this->gProperty->Deactivate();
	Entity::Deactivate();
}

void ModelEntitySpotLight::Update()
{
	Entity::Update();
	this->SetTransform(Entity::GetTransform());
	Render::LightServer::Instance()->CalculateSpotlight(*this->lightEntity);
	Render::LightServer::Instance()->UpdateSpotLightBuffer();
}

void ModelEntitySpotLight::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
    this->SetSpotLightPosition(t);
}

void ModelEntitySpotLight::SetSpotLightPosition(const Math::mat4& t)
{
	this->lightEntity->position = t.get_position();
}

void ModelEntitySpotLight::SetSpotLightLength(const float& length)
{
    this->lightEntity->length = length;
}

void ModelEntitySpotLight::SetSpotLightAngle(const float& angle)
{
    this->lightEntity->angle = angle;
}

void ModelEntitySpotLight::SetSpotLightDirection(const Math::vec4& direction)
{
    this->lightEntity->coneDirection = direction;
}

void ModelEntitySpotLight::SetSpotLightColor(const Math::vec4& color)
{
    this->lightEntity->color = color;
}
}