#pragma once
#include <memory>
#include "shaderobject.h"
#include "textureresource.h"
#include "foundation/util/array.h"
#include "surface.h"
#include <map>
#include "foundation/util/variable.h"


namespace Render
{
	struct MaterialParameter
	{
		std::string name;
		Util::Variable var;
	};

class Material
{
public:
	Material();
	~Material();
/*
    void SetName();
    std::string GetName();

    void SetShaderObject();
    std::shared_ptr<ShaderObject> GetShaderObject();

    Util::Array<std::shared_ptr<TextureResource>>& TextureList();

    MaterialParameter& GetParameterByName(const std::string& name);

    Util::Array<Surface>& SurfaceList();
*/


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

inline Material::Material()
{
}

inline Material::~Material()
{
}

}