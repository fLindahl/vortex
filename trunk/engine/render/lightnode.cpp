#include "config.h"
#include "lightnode.h"

namespace Render
{

LightNode::LightNode()
{

}

LightNode::~LightNode()
{

}

void LightNode::setLightPosition(GLfloat x, GLfloat y, GLfloat z)
{
	lightPosition.set(x, y, z);
}

void LightNode::setCameraPosition(GLfloat x, GLfloat y, GLfloat z)
{
	cameraPosition.set(x, y, z);
}

Math::Vector3 LightNode::getCameraPosition()
{
	return cameraPosition;
}

void LightNode::setAmbientIntensity(GLfloat r, GLfloat g, GLfloat b)
{
	lightAmbientIntensity.set(r,g,b);
}

void LightNode::setDiffuseIntensity(GLfloat r, GLfloat g, GLfloat b)
{
	lightDiffuseIntensity.set(r, g, b);
}

void LightNode::setSpecularIntensity(GLfloat r, GLfloat g, GLfloat b)
{
	lightSpecularIntensity.set(r, g, b);
}

void LightNode::applyToShader(std::shared_ptr<ShaderObject> shader)
{
	shader->setUniVector3fv(this->lightPosition, "lightPosition");
	shader->setUniVector3fv(this->cameraPosition, "cameraPosition");

	shader->setUniVector3fv(this->lightAmbientIntensity, "u_lightAmbientIntensity");
	shader->setUniVector3fv(this->lightDiffuseIntensity, "u_lightDiffuseIntensity");
	shader->setUniVector3fv(this->lightSpecularIntensity, "u_lightSpecularIntensity");
}

}