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