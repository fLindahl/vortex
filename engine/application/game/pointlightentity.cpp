#include "config.h"
#include "pointlightentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{
PointLightEntity::PointLightEntity() :
		Entity()
{
    this->gProperty = new Render::GraphicsProperty();
	this->lightEntity = &Render::LightServer::Instance()->CreatePointLight(Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), this->GetTransform().get_position(), 3.0f);
}

PointLightEntity::~PointLightEntity()
{
    delete this->gProperty;
    Render::LightServer::Instance()->RemovePointLight(this->lightEntity);
}

void PointLightEntity::SetModel(std::shared_ptr<Render::ModelInstance> mdl)
{
    this->gProperty->setModelInstance(mdl);
}

void PointLightEntity::Activate()
{
    this->gProperty->SetOwner(this->shared_from_this());
    this->gProperty->Activate();
    Entity::Activate();
}

void PointLightEntity::Deactivate()
{
    this->gProperty->Deactivate();
    Entity::Deactivate();
}

void PointLightEntity::Update()
{
    Entity::Update();
}

void PointLightEntity::SetTransform(const Math::mat4& nTransform)
{
    Entity::SetTransform(nTransform);
    this->gProperty->setModelMatrix(nTransform);
    this->SetPointLightPosition(nTransform);
}

void PointLightEntity::SetPointLightPosition(const Math::mat4& t)
{
    this->lightEntity->position = t.get_position();
	Render::LightServer::Instance()->UpdatePointLightBuffer();
}

void PointLightEntity::SetPointLightRadius(const float& radius)
{
    this->lightEntity->radiusAndPadding.x() = radius;
	Render::LightServer::Instance()->UpdatePointLightBuffer();
}

void PointLightEntity::SetPointLightColor(const Math::vec4& color)
{
    this->lightEntity->color.set(color.x(), color.y(), color.z());
	Render::LightServer::Instance()->UpdatePointLightBuffer();
}
};