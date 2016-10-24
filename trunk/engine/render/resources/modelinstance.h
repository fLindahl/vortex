#pragma once
#include <memory>
#include "foundation/util/array.h"

namespace Util
{
	class String;
}

namespace Render
{

class MeshResource;
class Material;
class GraphicsProperty;

class ModelInstance
{
public:
	ModelInstance();
	~ModelInstance();

	std::shared_ptr<MeshResource> GetMesh();
	void SetMesh(const char* file);
	
	void AddGraphicsProperty(GraphicsProperty* gp);

	void SetMaterial(const Util::String& name);
	std::shared_ptr<Material> GetMaterial();

	Util::Array<GraphicsProperty*>& GetGraphicsProperties();

private:
	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<Material> material;

	Util::Array<GraphicsProperty*> graphicsProperties;
};

}