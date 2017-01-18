#pragma once
#include <memory>
#include <foundation/math/bbox.h>
#include "foundation/util/array.h"
#include <string>

namespace Util
{
	class String;
}

namespace Render
{

class MeshResource;
class Material;
class Surface;
class GraphicsProperty;
class ModelInstance;

struct ModelNode
{
	~ModelNode();
	//index for the primitivegroup to render
	uint primitiveGroup;
	//pointer back to the surface
	Surface* surface;
	//pointer to the modelinstance that own this modelnode
	ModelInstance* modelInstance;
};

class ModelInstance
{
public:
	ModelInstance();
	~ModelInstance();
	
	std::shared_ptr<MeshResource> GetMesh();

	///only call this if you know what you're doing!
	void SetMesh(const char* file);

	void AddGraphicsProperty(GraphicsProperty* gp);
	void RemoveGraphicsProperty(GraphicsProperty* gp);

	//void SetMaterial(const Util::String& name);
	//std::shared_ptr<Material> GetMaterial();

	//void SetSurface(const Util::String& name);
	//void SetSurfaceList(Util::Array<std::shared_ptr<Surface>> list);
	//const Util::Array<std::shared_ptr<Surface>>& GetSurfaces();

	const Util::Array<ModelNode*>& GetModelNodes() { return this->modelNodes; }

	Util::Array<GraphicsProperty*>& GetGraphicsProperties();

	std::string GetName() { return this->name; }

private:
	friend class ResourceServer;

	std::string name;

	Util::Array<ModelNode*> modelNodes;

	std::shared_ptr<MeshResource> mesh;

	//Util::Array<std::shared_ptr<Surface>> surfaces;
	//std::shared_ptr<Material> material;

	Util::Array<GraphicsProperty*> graphicsProperties;
};

}