#pragma once
#include "GL/glew.h"
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

	//void setRasterizer(std::shared_ptr<Rasterizer> inRaster);

	void loadFromFile(const char * filename);
	//void loadFromRasterizer();
	void BindTexture(GLuint slot);

private:
	std::shared_ptr<ShaderObject> shader;
	//std::shared_ptr<Rasterizer> raster;

	GLuint texture;
};

}