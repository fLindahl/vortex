#pragma once
#include <memory>
#include "shaderobject.h"
#include "textureresource.h"
#include "foundation/util/array.h"
#include "surface.h"
#include <map>

namespace Render
{

class Material
{
public:
	Material();
	~Material();




private:
	// name of the material
	std::string name;

	// loaded shader object
	std::shared_ptr<ShaderObject> shader;

	// loaded textures
	Util::Array<std::shared_ptr<TextureResource>> textures;
	
	std::map<std::string, MaterialParameter> parametersByName;

	// all surfaces that currently use this material
	Util::Array<Surface> surfaces;
};

Material::Material()
{
}

Material::~Material()
{
}

}