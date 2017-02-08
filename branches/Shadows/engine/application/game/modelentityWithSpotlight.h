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

    Render::SpotLight* GetSpotLightEnity(){ return this->spotlightEnt; } const
    Render::SpotLight* SetSpotLightEnity(Render::SpotLight* sLight){ this->spotlightEnt = sLight; }

    void SetIndex(const uint& index){ this->index = index; }
    uint GetIndex(){ return this->index; }

protected:
    Render::SpotLight* spotlightEnt;
    Render::GraphicsProperty* gProperty;

    /// Index of the Light in the SpotLight Array
    uint index;
};
}