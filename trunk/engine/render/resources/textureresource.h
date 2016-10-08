#pragma once
#include "GL/glew.h"
#include "foundation/math/vector3.h"
#include <memory>
#include "shaderobject.h"
#include "render/softwarerender/rasterizer.h"

namespace Render
{

class TextureResource
{
public:
	TextureResource();
	~TextureResource();

	void setRasterizer(shared_ptr<Rasterizer> inRaster);

	void loadFromFile(const char * filename);
	void loadFromRasterizer();
	void BindTexture(GLuint slot);

private:
	shared_ptr<ShaderObject> shader;
	shared_ptr<Rasterizer> raster;

	GLuint texture;
};

}