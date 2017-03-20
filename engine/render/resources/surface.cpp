#include "config.h"
#include "render/server/resourceserver.h"
#include "surface.h"

namespace Render
{

Surface::Surface()
{
}

Surface::~Surface()
{
}

void Surface::SetupSurface(const Util::String &file)
{

}

void Surface::AddParameter(const Util::String& name, const Util::Variable& variable)
{
    if (variable.GetType() == Util::Variable::Type::String)
    {
		//HACK: Since we're loading a string, we're probably loading a texture
        //This should probably be done in some other way
		std::shared_ptr<TextureResource> tex = ResourceServer::Instance()->LoadTexture(*variable.GetString());
		this->textures.Append(tex);
		this->texturesByType.insert(std::make_pair(Material::TextureTypeFromString(name.AsCharPtr()), tex));
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

bool Surface::RemoveModelNode(ModelNode* node)
{
	auto n = this->modelNodes.Find(node);
	if (n != nullptr)
	{
		this->modelNodes.Remove(n);
		return true;
	}
	return false;
}

Util::Array<std::shared_ptr<TextureResource>>& Surface::TextureList()
{
    return this->textures;
}

MaterialParameter* Surface::GetParameterByName(const Util::String &name)
{
	return this->parametersByName[name];
}

std::shared_ptr<Material> Surface::GetMaterial()
{
    return this->material;
}

Util::String Surface::GetName()
{
    return this->name;
}

}