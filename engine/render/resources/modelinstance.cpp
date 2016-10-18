#include "config.h" 
#include "modelinstance.h"
#include "render/resources/shaderobject.h"
#include "render/resources/meshresource.h"
#include "render/resources/textureresource.h"
#include "render/server/resourceserver.h"

namespace Render
{

ModelInstance::ModelInstance()
{
	mesh = nullptr;
	texture = nullptr;
	shaderObject = nullptr;
}

ModelInstance::~ModelInstance()
{
	if (this->shaderObject != nullptr)
	{
		//HACK: Implement this
		//this->shaderObject->removeModelInstance(this);
	}
}

shared_ptr<ShaderObject> ModelInstance::getShaderObject()
{
	return this->shaderObject;
}

void ModelInstance::setShaderObject(shared_ptr<ShaderObject> inShaderObject)
{
	if (this->shaderObject != nullptr)
	{
		//HACK: Implement this
		//this->shaderObject->removeModelInstance(this);
	}

	this->shaderObject = inShaderObject;
	this->shaderObject->getModelInstances().Append(this);
}

shared_ptr<MeshResource> ModelInstance::getMesh()
{
	return this->mesh;
}

void ModelInstance::setMesh(const char* file)
{
	this->mesh = ResourceServer::Instance()->LoadMesh(file);
}

shared_ptr<TextureResource> ModelInstance::getTexture()
{
	return this->texture;
}

void ModelInstance::setTexture(const char* file)
{
	this->texture = ResourceServer::Instance()->LoadTexture(file);
}

void ModelInstance::addGraphicsProperty(GraphicsProperty* gp)
{
	this->graphicsProperties.Append(gp);
}

Util::Array<GraphicsProperty*>& ModelInstance::getGraphicsProperties()
{
	return this->graphicsProperties;
}

/*

this->shader->setUniVector3fv(this->matAmbientReflectance, "u_matAmbientReflectance");
this->shader->setUniVector3fv(this->matDiffuseReflectance, "u_matDiffuseReflectance");
this->shader->setUniVector3fv(this->matSpecularReflectance, "u_matSpecularReflectance");

this->shader->setUni1f(this->matShininess, "u_matShininess");


*/


}
