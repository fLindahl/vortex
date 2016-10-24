#include "config.h" 
#include "modelinstance.h"
#include "render/resources/meshresource.h"
#include "render/server/resourceserver.h"

namespace Render
{

ModelInstance::ModelInstance()
{
	mesh = nullptr;
	material = nullptr;
}

ModelInstance::~ModelInstance()
{
	if (this->material != nullptr)
	{
		//HACK: Implement this
		//this->material->removeModelInstance(this);
	}
}

shared_ptr<Material> ModelInstance::GetMaterial()
{
	return this->material;
}

void ModelInstance::SetMaterial(const Util::String& name)
{
	if (this->material != nullptr)
	{
		//HACK: Implement this
		//this->material->removeModelInstance(this);
	}

	this->material = ResourceServer::Instance()->GetMaterial(name);
	this->material->getModelInstances().Append(this);
}

shared_ptr<MeshResource> ModelInstance::GetMesh()
{
	return this->mesh;
}

void ModelInstance::SetMesh(const char* file)
{
	this->mesh = ResourceServer::Instance()->LoadMesh(file);
}

void ModelInstance::AddGraphicsProperty(GraphicsProperty* gp)
{
	this->graphicsProperties.Append(gp);
}

Util::Array<GraphicsProperty*>& ModelInstance::GetGraphicsProperties()
{
	return this->graphicsProperties;
}

}
