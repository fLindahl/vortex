#pragma once
#include "GL/glew.h"
#include <memory>
#include "shaderobject.h"
//#include "render/softwarerender/rasterizer.h"
#include "foundation/util/string.h"

namespace Render
{

class TextureResource : public Core::RefCounted
{
__DeclareClass(TextureResource);
public:
	TextureResource();
	~TextureResource();

	void loadFromFile(const char * filename);
	void BindTexture(GLuint slot);

	GLuint GetHandle() { return this->texture; }

	std::string GetName();

private:
	friend class ResourceServer;

	Util::String name;

	Ptr<ShaderObject> shader;

	GLuint texture;
};

}