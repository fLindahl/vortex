#pragma once
#include "foundation/math/vector4.h"
#include "GL/glew.h"
#include <memory>
#include "shaderobject.h"

namespace Render
{
class LightNode
{
public:
	LightNode();
	~LightNode();

	void setLightPosition(GLfloat x, GLfloat y, GLfloat z);
	void setCameraPosition(GLfloat x, GLfloat y, GLfloat z);
	
	void setAmbientIntensity(GLfloat r, GLfloat g, GLfloat b);
	void setDiffuseIntensity(GLfloat r, GLfloat g, GLfloat b);
	void setSpecularIntensity(GLfloat r, GLfloat g, GLfloat b);

	void applyToShader(std::shared_ptr<ShaderObject> shader);
	
private:
	/*
	Math::Vector3 lightPosition;
	Math::Vector3 cameraPosition = Math::Vector3(0.0f, 0.0f, 0.0f);

	// parameters of the light and possible values
	Math::Vector3 lightAmbientIntensity = Math::Vector3(0.2f, 0.2f, 0.2f);
	Math::Vector3 lightDiffuseIntensity = Math::Vector3(0.8f, 0.8f, 0.8f);
	Math::Vector3 lightSpecularIntensity = Math::Vector3(0.8f, 0.8f, 0.5f);
	*/
};
}