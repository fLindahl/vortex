#include "config.h"
#include "material.h"
#include "render/server/resourceserver.h"
#include "render/server/frameserver.h"
#include "render/frame/drawpass.h"

namespace Render
{
	Material::Material()
	{

	}

	Material::~Material()
	{

	}

	/*
	bool Material::operator<(const Material& rhs) const
	{
		//HACK: This is currently used to sort material by shader, should be changed later
		return (this->shader->GetProgram() < rhs.GetShader()->GetProgram());
	}
	*/

	void Material::SetName(const Util::String& n)
	{
		this->name = n;
	}

	std::shared_ptr<ShaderObject> Material::GetShader(const std::string& pass)
	{
		return this->framepasses[Util::String(pass.c_str())];
	}

	Util::String Material::GetName()
	{
		return this->name;
	}

	Util::Array<std::shared_ptr<TextureResource>>& Material::TextureList()
	{
		return this->textures;
	}

	MaterialParameter* Material::GetParameterByName(const Util::String& name)
	{
		if (this->parametersByName.count(name))
		{
			return this->parametersByName[name];
		}
		else
		{
			printf("ERROR: Could not find parameter with name %s!", name.AsCharPtr());

#ifdef _DEBUG
			assert(false);
#endif // _DEBUG

			return nullptr;
		}
	}

	void Material::AddParameter(const Util::String& name, const Util::Variable& variable)
	{
		if (variable.GetType() == Util::Variable::Type::String)
		{
			//HACK: Since we're loading a string, we're probably loading a texture
			//This should probably be done in some other way
			const Util::String str = *variable.GetString();
			this->textures.Append(ResourceServer::Instance()->LoadTexture(str));
			this->TextureParamTypes.Append(Material::TextureTypeFromString(name.AsCharPtr()));
		}
		else
		{
			MaterialParameter* param = new MaterialParameter();
			param->name = name;
			param->var = variable;
			this->parametersByName.insert(std::make_pair(name, param));
			parameters.Append(param);
		}
	}

	Util::Array<std::shared_ptr<Surface>>& Material::SurfaceList()
	{
		return this->surfaces;
	}

	void Material::SetFramePass(const Util::String& pass, const Util::String& shader)
	{
		if (FrameServer::Instance()->HasPassNamed(pass.AsCharPtr()))
		{
			auto drawpass = std::dynamic_pointer_cast<DrawPass>(FrameServer::Instance()->GetFramePass(pass.AsCharPtr()));
			
			//Make sure we can actually cast
			assert(drawpass != nullptr);

			drawpass->AddMaterial(this);
			this->framepasses.insert(std::make_pair(pass, ShaderServer::Instance()->LoadShader(std::string(shader.AsCharPtr()))));
		}
		else
		{
			printf("ERROR Material::SetFramePass : Cannot find pass named %s!\n", pass.AsCharPtr());
			assert(false);
		}		
	}

	TextureType Material::TextureTypeFromString(const std::string& s)
	{
		if (s == VORTEX_SEMANTIC_ALBEDOMAP)
		{
			return TextureType::AlbedoMap;
		}
		if (s == VORTEX_SEMANTIC_NORMALMAP)
		{
			return TextureType::NormalMap;
		}
		if (s == VORTEX_SEMANTIC_SPECULARMAP)
		{
			return TextureType::SpecularMap;
		}
		if (s == VORTEX_SEMANTIC_ROUGHNESSMAP)
		{
			return TextureType::RoughnessMap;
		}
		else
		{
			_assert(false, "ERROR: Texture type name does not exist in semantics!");
			return TextureType::AlbedoMap;
		}
	}
}
