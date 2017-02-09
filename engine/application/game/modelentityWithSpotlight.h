/*
 * The Spotlights are added in commands.h
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

	float& GetSpotLightLength(){ return this->spotlightEnt->length; }
	float& GetSpotLightAngle(){ return this->spotlightEnt->angle; }
	Math::vec4& GetSpotLightDirection(){ return this->spotlightEnt->coneDirection; }
	Math::vec4& GetSpotLightColor(){ return this->spotlightEnt->color; }

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

    Render::LightServer::SpotLight* GetSpotLightEnity(){ return this->spotlightEnt; } const
    Render::LightServer::SpotLight* SetSpotLightEnity(Render::LightServer::SpotLight* sLight){ this->spotlightEnt = sLight; }

    /*void SetLightIndex(const uint& index){ this->lightIndex = index; }
    uint GetLightIndex(){ return this->lightIndex; }*/

protected:
    Render::LightServer::SpotLight* spotlightEnt;
    Render::GraphicsProperty* gProperty;
};
}