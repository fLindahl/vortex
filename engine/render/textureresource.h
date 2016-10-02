#pragma once
#include "GL/glew.h"
#include "foundation/math/vector3.h"
#include <memory>
#include "shaderobject.h"
#include "rasterizer.h"

namespace Render
{

class TextureResource
{
public:
	TextureResource();
	~TextureResource();

	shared_ptr<ShaderObject> getShader();
	void setShader(shared_ptr<ShaderObject> inShader);
	void setRasterizer(shared_ptr<Rasterizer> inRaster);

	void loadFromFile(const char * filename);
	void loadFromRasterizer();
	void bindTexture(GLuint slot);

	void setAmbientReflectance(GLfloat r, GLfloat g, GLfloat b);
	void setDiffuseReflectance(GLfloat r, GLfloat g, GLfloat b);
	void setSpecularReflectance(GLfloat r, GLfloat g, GLfloat b);
	void setShininess(GLfloat value);

private:
	shared_ptr<ShaderObject> shader;
	shared_ptr<Rasterizer> raster;

	GLuint texture;

	//THIS SHOULD BE MATERIAL SPECIFIC. NOT LIGHT SPECIFIC
	// parameters of the material and possible values
	Math::Vector3 matAmbientReflectance = Math::Vector3(1, 1, 1);
	Math::Vector3 matDiffuseReflectance = Math::Vector3(1, 1, 1);
	Math::Vector3 matSpecularReflectance = Math::Vector3(1, 1, 1);
	GLfloat matShininess = 64;
};

}