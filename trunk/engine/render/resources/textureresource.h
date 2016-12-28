#pragma once
#include "GL/glew.h"
#include <memory>
#include "shaderobject.h"
//#include "render/softwarerender/rasterizer.h"
#include "foundation/util/string.h"

namespace Render
{

class TextureResource
{
public:
	TextureResource();
	~TextureResource();

	void loadFromFile(const char * filename);
	void BindTexture(GLuint slot);

	std::string GetName();

private:
	friend class ResourceServer;

	Util::String name;

	std::shared_ptr<ShaderObject> shader;

	GLuint texture;
};

}