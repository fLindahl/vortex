#include "config.h"
#include "cubemapnode.h"

namespace Render
{

CubeMapNode::CubeMapNode()
{

}

CubeMapNode::~CubeMapNode()
{

}

void CubeMapNode::setLightPosition(GLfloat x, GLfloat y, GLfloat z)
{
	//lightPosition.set(x, y, z);
}

void CubeMapNode::setCameraPosition(GLfloat x, GLfloat y, GLfloat z)
{
	//cameraPosition.set(x, y, z);
}

void CubeMapNode::setAmbientIntensity(GLfloat r, GLfloat g, GLfloat b)
{
	//lightAmbientIntensity.set(r,g,b);
}

void CubeMapNode::setDiffuseIntensity(GLfloat r, GLfloat g, GLfloat b)
{
	//lightDiffuseIntensity.set(r, g, b);
}

void CubeMapNode::setSpecularIntensity(GLfloat r, GLfloat g, GLfloat b)
{
	//lightSpecularIntensity.set(r, g, b);
}

void CubeMapNode::applyToShader(std::shared_ptr<ShaderObject> shader)
{
	//shader->setUniVector3fv(this->lightPosition, "lightPosition");
	//shader->setUniVector3fv(this->cameraPosition, "cameraPosition");

	//shader->setUniVector3fv(this->lightAmbientIntensity, "u_lightAmbientIntensity");
	//shader->setUniVector3fv(this->lightDiffuseIntensity, "u_lightDiffuseIntensity");
	//shader->setUniVector3fv(this->lightSpecularIntensity, "u_lightSpecularIntensity");
}

}