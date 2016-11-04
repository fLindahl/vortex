#include "config.h"
#include "material.h"
#include "render/server/resourceserver.h"

namespace Render
{
	Material::Material()
	{

	}

	Material::~Material()
	{

	}

	bool Material::operator<(const Material& rhs) const
	{
		//HACK: This is currently used to sort material by shader, should be changed later
		return (this->shader->GetProgram() < rhs.GetShader()->GetProgram());
	}

	void Material::SetName(const Util::String& n)
	{
		this->name = n;
	}

	Util::String Material::GetName()
	{
		return this->name;
	}

	void Material::SetShader(const Util::String& name)
	{
		//HACK: shouldn't need to cast to std::string...
		this->shader = ShaderServer::Instance()->LoadShader(std::string(name));
	}

	std::shared_ptr<ShaderObject> Material::GetShader()	const
	{
		return this->shader;
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

	void Material::SetFramePass(const Util::String& pass)
	{
		//TODO: Implement!
	}

	index_t Material::GetFramePass()
	{
		return this->framePass;
	}

}
