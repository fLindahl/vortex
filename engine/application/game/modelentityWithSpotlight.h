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
	__DeclareClass(ModelEntitySpotLight)
public:
	ModelEntitySpotLight();
	~ModelEntitySpotLight();

	void SetModel(Ptr<Render::ModelInstance> mdl);

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();
	void Update();

	void SetTransform(const Math::mat4& t);

	const float& GetSpotLightLength(){ return this->length; }
	void SetSpotLightLength(const float& length);
	
	const float& GetSpotLightAngle(){ return this->angle; }
    void SetSpotLightAngle(const float& angle);

	const Math::vec4& GetSpotLightDirection(){ return this->transform.get_zaxis(); }
    //void SetSpotLightDirection(const Math::vec4& direction);
	
	Math::vec4 GetSpotLightColor(){ return Math::vec4(this->lightEntity->colorAndCenterOffset.x(), this->lightEntity->colorAndCenterOffset.y(), this->lightEntity->colorAndCenterOffset.z(), 1.0f); }
    void SetSpotLightColor(const Math::vec4& color);

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

    Render::LightServer::SpotLight* GetSpotLightEnity(){ return this->lightEntity; } const
    void SetSpotLightEnity(Render::LightServer::SpotLight* sLight){ this->lightEntity = sLight; }

protected:
	//entitys transform is the position and orientation of the light
	float length;
	float angle;

    Render::LightServer::SpotLight* lightEntity;
    Render::GraphicsProperty* gProperty;
};
}