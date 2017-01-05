#include "config.h" 
#include "modelinstance.h"
#include "render/resources/meshresource.h"
#include "render/server/resourceserver.h"
#include "render/resources/surface.h"

namespace Render
{

ModelInstance::ModelInstance()
{
	mesh = nullptr;
	surface = nullptr;
	//material = nullptr;
}

ModelInstance::~ModelInstance()
{
	//if (this->material != nullptr)
	//{
		//HACK: Implement this
		//this->material->removeModelInstance(this);
	//}
}

std::shared_ptr<Surface> ModelInstance::GetSurface()
{
	return this->surface;
}

void ModelInstance::SetSurface(const Util::String& name)
{
	if (this->surface != nullptr)
	{
		//HACK: Implement this
		//this->material->removeModelInstance(this);
	}

	this->surface = ResourceServer::Instance()->LoadSurface(name);
	this->surface->getModelInstances().Append(this);
}

//std::shared_ptr<Material> ModelInstance::GetMaterial()
//{
//	return this->material;
//}
//
//void ModelInstance::SetMaterial(const Util::String& name)
//{
//	if (this->material != nullptr)
//	{
//		//HACK: Implement this
//		//this->material->removeModelInstance(this);
//	}
//
//	this->material = ResourceServer::Instance()->GetMaterial(name);
//	this->material->getModelInstances().Append(this);
//}



std::shared_ptr<MeshResource> ModelInstance::GetMesh()
{
	return this->mesh;
}

void ModelInstance::SetMesh(const char* file)
{
	this->mesh = ResourceServer::Instance()->LoadMesh(file);
}

void ModelInstance::AddGraphicsProperty(GraphicsProperty* gp)
{
	auto it = this->graphicsProperties.Find(gp);
	if (it == nullptr)
	{
		this->graphicsProperties.Append(gp);
	}
	else
	{
		printf("WARNING: GraphicsProperty already registered to this Model!\n");
	}
}

void ModelInstance::RemoveGraphicsProperty(GraphicsProperty* gp)
{
	auto it = this->graphicsProperties.Find(gp);
	if (it != nullptr)
	{
		//Erase and move last element to this position.
		//Destroys sorting!
		it = this->graphicsProperties.EraseSwap(it);
	}
}

Util::Array<GraphicsProperty*>& ModelInstance::GetGraphicsProperties()
{
	return this->graphicsProperties;
}

}
