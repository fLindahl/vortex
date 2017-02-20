#include "config.h"
#include "modelentityWithSpotlight.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/debugrender/debugrenderer.h"
#include "render/server/shadowserver.h"

namespace Game
{

ModelEntitySpotLight::ModelEntitySpotLight() : 
		Entity(),
		angle(50.0f),
		length(5.0f)
{
	this->gProperty = new Render::GraphicsProperty();
	this->lightEntity = &Render::LightServer::Instance()->CreateSpotLight(Math::vec4(1.0f, 1.0f, 1.0f, 1.0f),
																		this->GetTransform().get_position(),
																		Math::vec4(0.0f, -1.0f, 0.0f, 1.0f),
																		5.0f,
																		50.0f);
}
	
ModelEntitySpotLight::~ModelEntitySpotLight()
{
	delete this->gProperty;
	Render::LightServer::Instance()->RemoveSpotLight(this->lightEntity);
}

void ModelEntitySpotLight::SetModel(std::shared_ptr<Render::ModelInstance> mdl)
{
	this->gProperty->setModelInstance(mdl);
}

void ModelEntitySpotLight::Activate()
{
	this->gProperty->SetOwner(this->shared_from_this());
	this->gProperty->Activate();

	Render::ShadowServer::Instance()->AppendSpotLightEntity(dynamic_cast<Game::ModelEntitySpotLight*>(this->shared_from_this().get()));

	Entity::Activate();
}

void ModelEntitySpotLight::Deactivate()
{
	this->gProperty->Deactivate();

	Render::ShadowServer::Instance()->RemoveSpotLightEntity(dynamic_cast<Game::ModelEntitySpotLight*>(this->shared_from_this().get()));

	Entity::Deactivate();
}

void ModelEntitySpotLight::Update()
{
	Entity::Update();

	//Draw bounding sphere
	//Math::point center = this->lightEntity->centerAndRadius;
	//float radius = this->lightEntity->centerAndRadius.w();
	//Debug::DebugRenderer::Instance()->DrawCircle(center, Math::quaternion::identity(), radius, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);
	//Debug::DebugRenderer::Instance()->DrawCircle(center, Math::quaternion::rotationyawpitchroll(0.0f, 1.57075f, 0.0f), radius, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);
	//Debug::DebugRenderer::Instance()->DrawCircle(center, Math::quaternion::rotationyawpitchroll(1.57075f, 0.0f, 1.57075f), radius, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);

}

void ModelEntitySpotLight::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
	this->gProperty->setModelMatrix(t);
	Render::LightServer::SpotLight nSpot = Render::LightServer::Instance()->CalculateSpotlight(this->lightEntity->colorAndCenterOffset, this->transform.get_position(), this->transform.get_zaxis(), this->length, this->angle);
	this->lightEntity->centerAndRadius = nSpot.centerAndRadius;
	this->lightEntity->colorAndCenterOffset = nSpot.colorAndCenterOffset;
	this->lightEntity->params = nSpot.params;
	Render::LightServer::Instance()->UpdateSpotLightBuffer();
}

void ModelEntitySpotLight::SetSpotLightLength(const float& length)
{
    this->length = length;
	Render::LightServer::SpotLight nSpot = Render::LightServer::Instance()->CalculateSpotlight(this->lightEntity->colorAndCenterOffset, this->transform.get_position(), this->transform.get_zaxis(), this->length, this->angle);
	this->lightEntity->centerAndRadius = nSpot.centerAndRadius;
	this->lightEntity->colorAndCenterOffset = nSpot.colorAndCenterOffset;
	this->lightEntity->params = nSpot.params;
	Render::LightServer::Instance()->UpdateSpotLightBuffer();
}

void ModelEntitySpotLight::SetSpotLightAngle(const float& angle)
{
    this->angle = angle;
	Render::LightServer::SpotLight nSpot = Render::LightServer::Instance()->CalculateSpotlight(this->lightEntity->colorAndCenterOffset, this->transform.get_position(), this->transform.get_zaxis(), this->length, this->angle);
	this->lightEntity->centerAndRadius = nSpot.centerAndRadius;
	this->lightEntity->colorAndCenterOffset = nSpot.colorAndCenterOffset;
	this->lightEntity->params = nSpot.params;
	Render::LightServer::Instance()->UpdateSpotLightBuffer();
}

void ModelEntitySpotLight::SetSpotLightColor(const Math::vec4& color)
{
    this->lightEntity->colorAndCenterOffset.x() = color.x();
	this->lightEntity->colorAndCenterOffset.y() = color.y();
	this->lightEntity->colorAndCenterOffset.z() = color.z();
	Render::LightServer::Instance()->UpdateSpotLightBuffer();
}
}