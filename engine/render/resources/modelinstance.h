#pragma once
#include <memory>
#include <foundation/math/bbox.h>
#include "foundation/util/array.h"

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

class ModelInstance
{
public:
	ModelInstance();
	~ModelInstance();

	std::shared_ptr<MeshResource> GetMesh();
	void SetMesh(const char* file);

	void AddGraphicsProperty(GraphicsProperty* gp);
	void RemoveGraphicsProperty(GraphicsProperty* gp);

	//void SetMaterial(const Util::String& name);
	//std::shared_ptr<Material> GetMaterial();

	void SetSurface(const Util::String& name);
	std::shared_ptr<Surface> GetSurface();

	Util::Array<GraphicsProperty*>& GetGraphicsProperties();

private:
	std::shared_ptr<MeshResource> mesh;

	std::shared_ptr<Surface> surface;
	//std::shared_ptr<Material> material;

	Util::Array<GraphicsProperty*> graphicsProperties;
};

}