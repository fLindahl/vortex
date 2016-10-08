#pragma once
#include <memory>
#include "foundation/util/array.h"

namespace Render
{

class MeshResource;
class TextureResource;
class ShaderObject;
class GraphicsProperty;

class ModelInstance
{
public:
	ModelInstance();
	~ModelInstance();

	std::shared_ptr<ShaderObject> getShaderObject();
	void setShaderObject(std::shared_ptr<ShaderObject> inShaderObject);

	std::shared_ptr<MeshResource> getMesh();
	void setMesh(std::shared_ptr<MeshResource> inMesh);

	std::shared_ptr<TextureResource> getTexture();
	void setTexture(std::shared_ptr<TextureResource> inTexture);
	
	void addGraphicsProperty(GraphicsProperty* gp);

	Util::Array<GraphicsProperty*>& getGraphicsProperties();

private:
	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<TextureResource> texture;
	std::shared_ptr<ShaderObject> shaderObject;

	Util::Array<GraphicsProperty*> graphicsProperties;
};

}