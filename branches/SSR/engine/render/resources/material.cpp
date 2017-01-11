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
			printf("ERROR: Could not find parameter with name %s!", name.c_str());

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
			this->textures.Append(ResourceServer::Instance()->LoadTexture(*variable.GetString()));
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
		if (FrameServer::Instance()->HasPassNamed(pass.c_str()))
		{
			auto drawpass = std::dynamic_pointer_cast<DrawPass>(FrameServer::Instance()->GetFramePass(pass.c_str()));
			
			//Make sure we can actually cast
			assert(drawpass != nullptr);

			drawpass->AddMaterial(this);
			this->framepasses.insert(std::make_pair(pass, ShaderServer::Instance()->LoadShader(std::string(shader.c_str()))));
		}
		else
		{
			printf("ERROR Material::SetFramePass : Cannot find pass named %s!\n", pass.c_str());
			assert(false);
		}		
	}

	/*
	index_t Material::GetFramePass()
	{
		return this->framePass;
	}
	*/
}
