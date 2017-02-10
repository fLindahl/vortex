/*
 * The Spotlights are added in commands.h
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

class ModelEntitySpotLight : public Entity
{
public:
	ModelEntitySpotLight();
	~ModelEntitySpotLight();

	void SetModel(std::shared_ptr<Render::ModelInstance> mdl);

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();
	void Update();

	void SetTransform(const Math::mat4& t);
	void SetSpotLightPosition(const Math::mat4& t);
    void SetSpotLightLength(const float& length);
    void SetSpotLightAngle(const float& angle);
    void SetSpotLightDirection(const Math::vec4& direction);
    void SetSpotLightColor(const Math::vec4& color);

	float& GetSpotLightLength(){ return this->lightEntity->length; }
	float& GetSpotLightAngle(){ return this->lightEntity->angle; }
	Math::vec4& GetSpotLightDirection(){ return this->lightEntity->coneDirection; }
	Math::vec4& GetSpotLightColor(){ return this->lightEntity->color; }

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

    Render::LightServer::SpotLight* GetSpotLightEnity(){ return this->lightEntity; } const
    void SetSpotLightEnity(Render::LightServer::SpotLight* sLight){ this->lightEntity = sLight; }

    void SetLightIndex(const uint& index){ this->lightIndex = index; }
    int GetLightIndex(){ return this->lightIndex; }

    Render::LightServer::LightType& GetLightType(){ return this->lightEntity->lightType; }

protected:
    Render::LightServer::SpotLight* lightEntity;
    Render::GraphicsProperty* gProperty;
};
}