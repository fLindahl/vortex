#include "config.h"
#include "resourceserver.h"
#include "tinyxml2.h"
#include "renderdevice.h"
#include "render/resources/surface.h"

namespace Render
{

std::shared_ptr<MeshResource> ResourceServer::LoadMesh(const std::string& meshpath)
{
	//Make sure we've not already loaded this model
	if (!this->HasMeshNamed(meshpath))
	{
		std::shared_ptr<MeshResource> nMesh = std::make_shared<MeshResource>();

        std::string fileExtension = meshpath.substr(meshpath.find_last_of("."));

        //if(fileExtension == ".obj")
        //{
        //    nMesh->loadMeshFromOBJ(meshpath.c_str());
        //}
        if(fileExtension == ".nvx2")
        {
            nMesh->loadMeshFromFile(meshpath.c_str());
        }
        else
        {
			nMesh->loadMesh(meshpath.c_str());
            //printf("Could not load mesh! Invalid file extension!");
            //assert(false);
            //_assert(false, "Could not load mesh!");
        }
		std::pair<const char*, std::shared_ptr<MeshResource>> par(meshpath.c_str(), nMesh);
		this->meshes.insert(par);

		return nMesh;
	}
	else
	{
		//Model is already loaded so we can just return that model.
		return this->meshes.find(meshpath)->second;
	}
}

bool ResourceServer::HasMeshNamed(const std::string& nName)
{
	//Because unordered_map containers do not allow for duplicate keys, this means that 
	//count function actually returns 1 if an element with that key exists in the container, and zero otherwise.
	if (this->meshes.count(nName) > 0)
		return true;
	else
		return false;
}

std::shared_ptr<TextureResource> ResourceServer::LoadTexture(const char* filepath)
{
	//Make sure we've not already loaded this texture
	if (!this->HasTextureNamed(filepath))
	{
		std::shared_ptr<TextureResource> texture = std::make_shared<TextureResource>();
		texture->loadFromFile(filepath);
		std::pair<const char*, std::shared_ptr<TextureResource>> par(filepath, texture);
		texture->name = filepath;
		this->textures.insert(par);

		return texture;
	}
	else
	{
		//texture is already loaded so we can just return that texture.
		return this->textures.find(filepath)->second;
	}
}


bool ResourceServer::HasTextureNamed(const std::string& nName)
{
	//Because unordered_map containers do not allow for duplicate keys, this means that 
	//count function actually returns 1 if an element with that key exists in the container, and zero otherwise.
	if (this->textures.count(nName) > 0)
		return true;
	else
		return false;
}

std::shared_ptr<Material> ResourceServer::GetMaterial(const Util::String& name)
{
	if (this->HasMaterialNamed(name.c_str()))
	{
		//HACK: We shouldn't need to cast here
		return this->materials[std::string(name)];
	}
	else
	{
		printf("ERROR: No such material!");
		return nullptr;
	}
}

bool ResourceServer::SetupMaterials(const char *fileName)
{
	//TODO: should this really be here?

	tinyxml2::XMLDocument data;
	int result = data.LoadFile(fileName);
	

	if (result != 0)
	{
		printf("ERROR: Could not load materials file!");

#ifdef DEBUG
		assert(false);
#endif // DEBUG		

		return false;
	}

	tinyxml2::XMLElement* materials = data.RootElement()->FirstChildElement();
	tinyxml2::XMLElement* material = materials->FirstChildElement();

	while (true)
	{
		const tinyxml2::XMLAttribute* nameAttr = material->FirstAttribute();

		//First we check if the specified material is already loaded.
		if (this->HasMaterialNamed(nameAttr->Value()))
		{
			printf("WARNING: Duplicate material loaded: \" %s \". Using previously loaded material...", nameAttr->Value());
		}
		else // Load material!
		{
			// Create our material
			std::shared_ptr<Material> mat = std::make_shared<Material>();

			// Set name
			mat->SetName(nameAttr->Value());

			// Get which frame pass we render in
			const tinyxml2::XMLElement* pass = material->FirstChildElement("Pass");
			while (true)
			{
				mat->SetFramePass(pass->FindAttribute("name")->Value(), pass->FindAttribute("shader")->Value());

				if (pass->NextSiblingElement("Pass") != nullptr)
				{
					pass = pass->NextSiblingElement("Pass");
				}
				else
				{
					// out of passes
					break;
				}
			}

			// Load shader
			//mat->SetShader(pass->FindAttribute("shader")->Value());

			// Get all parameters that this material contains
			const tinyxml2::XMLElement* param = material->FirstChildElement("Param");
			while (true)
			{
				Util::String type = param->FindAttribute("type")->Value();
								
				Util::Variable var = Util::Variable(Util::Variable::StringToType(type), Util::String(param->FindAttribute("defaultValue")->Value()));
				
				mat->AddParameter(param->FindAttribute("name")->Value(), var);
				
				if (param->NextSiblingElement("Param") != nullptr)
				{
					param = param->NextSiblingElement("Param");
				}
				else
				{
					// out of parameters
					break;
				}
			}
			
			this->materials.insert(std::make_pair(nameAttr->Value(), mat));
			RenderDevice::Instance()->AddMaterial(mat.get());
		}

		if (material->NextSiblingElement() != nullptr)
		{
			material = material->NextSiblingElement();
		}
		else
		{
			// end of materials
			break;
		}
	}

	return true;
}

bool ResourceServer::HasMaterialNamed(const std::string &nName)
{
	if (this->materials.count(nName) > 0)
		return true;
	else
		return false;
}

std::shared_ptr<Surface> ResourceServer::LoadSurface(const char* filepath)
{
    if(this->HasSurfaceNamed(filepath))
        return this->surfaces[filepath];

    tinyxml2::XMLDocument data;
    int result = data.LoadFile(filepath);

    if (result != 0)
    {
        printf("ERROR: Could not load materials file!");

#ifdef DEBUG
        assert(false);
#endif // DEBUG

        return false;
    }

    tinyxml2::XMLElement* surface = data.RootElement()->FirstChildElement();

    const tinyxml2::XMLAttribute* materialTemplate = surface->FirstAttribute();

    std::shared_ptr<Surface> sur = nullptr;

    //First we check if the specified material is loaded.
    if (!this->HasMaterialNamed(materialTemplate->Value()))
    {
        printf("ERROR: No material with name: %s!\n", materialTemplate->Value());
        assert(false);
    }
    else // Load surface!
    {
        // get material
        std::shared_ptr<Material> mat = this->GetMaterial(materialTemplate->Value());

        // create our surface
        sur = std::make_shared<Surface>();

		//Append this surface to materials surfacelist
		mat->surfaces.Append(sur);

        sur->name = filepath;
        sur->material = mat;

        // Get all parameters that this surface overrides
        const tinyxml2::XMLElement* param = surface->FirstChildElement("Param");
		while (param != nullptr)
        {
            Util::String type = param->FindAttribute("type")->Value();

            Util::Variable var = Util::Variable(Util::Variable::StringToType(type), Util::String(param->FindAttribute("value")->Value()));

            sur->AddParameter(param->FindAttribute("name")->Value(), var);

            if (param->NextSiblingElement("Param") != nullptr)
            {
                param = param->NextSiblingElement("Param");
            }
            else
            {
                // out of parameters
                break;
            }
        }

        // Now we add the additional parameters and textures that we've not overloaded.
        for (MaterialParameter* param : mat->parameters)
        {
            if (sur->parametersByName.count(param->name) == 0)
            {
                sur->parameters.Append(param);
                sur->parametersByName.insert(std::make_pair(param->name, param));
            }
        }

		for (auto tex : mat->textures)
		{
			if (sur->texturesByName.count(tex->name) == 0)
			{
				sur->textures.Append(tex);
				sur->texturesByName.insert(std::make_pair(tex->name, tex));
			}
		}
    }

    return sur;
}

bool ResourceServer::HasSurfaceNamed(const std::string &nName)
{
    if (this->surfaces.count(nName) > 0)
        return true;
    else
        return false;
}




}

