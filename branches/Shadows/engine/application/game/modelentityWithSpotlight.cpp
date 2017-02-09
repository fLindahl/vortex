#include "config.h"
#include "modelentityWithSpotlight.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{

ModelEntitySpotLight::ModelEntitySpotLight()//: spotlightEnt(Render::SpotLight())
{
	//Call baseclass first!
	Entity();
	this->gProperty = new Render::GraphicsProperty();
    this->spotlightEnt = new Render::LightServer::SpotLight();
/*
    Render::SpotLight spotlight;
    spotlight.position = Math::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    spotlight.color = Math::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    spotlight.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    spotlight.length = 5.0f;
    spotlight.angle = 15.0f;
    this->spotlightEnt = spotlight;
    Render::LightServer::Instance()->AddSpotLight(spotlight);*/
}
	
ModelEntitySpotLight::~ModelEntitySpotLight()
{
	delete this->gProperty;
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

    Render::LightServer::Instance()->Update();
}

void ModelEntitySpotLight::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
    this->SetSpotLightPosition(t);
}

void ModelEntitySpotLight::SetSpotLightPosition(const Math::mat4& t)
{
	this->spotlightEnt->position = t.get_position();
}

void ModelEntitySpotLight::SetSpotLightLength(const float& length)
{
    this->spotlightEnt->length = length;
}

void ModelEntitySpotLight::SetSpotLightAngle(const float& angle)
{
    this->spotlightEnt->angle = angle;
}

void ModelEntitySpotLight::SetSpotLightDirection(const Math::vec4& direction)
{
    this->spotlightEnt->coneDirection = direction;
}

void ModelEntitySpotLight::SetSpotLightColor(const Math::vec4& color)
{
    this->spotlightEnt->color = color;
}

}