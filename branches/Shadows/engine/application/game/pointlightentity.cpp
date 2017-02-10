#include "config.h"
#include "pointlightentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{
PointLightEntity::PointLightEntity()
{
    Entity();
    this->gProperty = new Render::GraphicsProperty();
    this->lightEntity = new Render::LightServer::PointLight();
}

PointLightEntity::~PointLightEntity()
{
    delete this->gProperty;
    delete this->lightEntity;
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
    this->SetTransform(Entity::GetTransform());
    Render::LightServer::Instance()->UpdatePointLightBuffer();
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
}

void PointLightEntity::SetPointLightRadius(const float& radius)
{
    this->lightEntity->radiusAndPadding.x() = radius;
}

void PointLightEntity::SetPointLightColor(const Math::vec4& color)
{
    this->lightEntity->color = color;
}
};