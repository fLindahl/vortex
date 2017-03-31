#pragma once
#include <memory>
#include <foundation/math/bbox.h>
#include "foundation/util/array.h"
#include <string>
#include "core/refcounted.h"

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

class ModelInstance : public Core::RefCounted
{
	__DeclareClass(ModelInstance);

public:
	ModelInstance();
	~ModelInstance();
	
	Ptr<MeshResource> GetMesh();

	///only call this if you know what you're doing!
	void SetMesh(const char* file);

	void AddGraphicsProperty(GraphicsProperty* gp);
	void RemoveGraphicsProperty(GraphicsProperty* gp);

	//void SetMaterial(const Util::String& name);
	//Ptr<Material> GetMaterial();

	//void SetSurface(const Util::String& name);
	//void SetSurfaceList(Util::Array<Ptr<Surface>> list);
	//const Util::Array<Ptr<Surface>>& GetSurfaces();

	const Util::Array<ModelNode*>& GetModelNodes() { return this->modelNodes; }

	Util::Array<GraphicsProperty*>& GetGraphicsProperties();

	std::string GetName() { return this->name; }

private:
	friend class ResourceServer;

	std::string name;

	Util::Array<ModelNode*> modelNodes;

	Ptr<MeshResource> mesh;

	//Util::Array<Ptr<Surface>> surfaces;
	//Ptr<Material> material;

	Util::Array<GraphicsProperty*> graphicsProperties;
};

}