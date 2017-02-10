/*
 * The Pointlights are added in commands.h
 *
 * TODO: Set functions should be moved to .h
 */

#pragma once
#include "entity.h"
#include "render/server/lightserver.h"


namespace Render
{
    class GraphicsProperty;
    class ModelInstance;
}

namespace Game
{
class PointLightEntity : public Entity
{
public:
    PointLightEntity();
    ~PointLightEntity();

    void SetModel(std::shared_ptr<Render::ModelInstance> mdl);

    void Activate();
    void Deactivate();
    void Update();

    void SetTransform(const Math::mat4 &nTransform);

    void SetPointLightPosition(const Math::mat4& t);
    void SetPointLightRadius(const float& radius);
    void SetPointLightColor(const Math::vec4& color);

    float& GetPointLightRadius(){ return this->lightEntity->radiusAndPadding.x(); }
    Math::vec4& GetPointLightColor(){ return this->lightEntity->color; }
    Math::vec4& SetPointLightPosition(){ return this->lightEntity->color; }

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

    Render::LightServer::PointLight* GetPointLightEnity(){ return this->lightEntity; } const
    void SetPointLightEnity(Render::LightServer::PointLight* sLight){ this->lightEntity = sLight; }

    void SetLightIndex(const uint& index){ this->lightIndex = index; }
    int GetLightIndex(){ return this->lightIndex; }

    Render::LightServer::LightType& GetLightType(){ return this->lightEntity->lightType; }

protected:
    Render::LightServer::PointLight* lightEntity;
    Render::GraphicsProperty* gProperty;
};
}