#include "config.h"
#include "resourceserver.h"
#include "tinyxml2.h"
#include "renderdevice.h"
#include "render/resources/surface.h"
#include "render/resources/textureresource.h"
#include "foundation/util/string.h"

namespace Render
{
ResourceServer::ResourceServer()
{

}

Ptr<MeshResource> ResourceServer::LoadMesh(const Util::String& meshpath)
{
	//Make sure we've not already loaded this model
	if (!this->HasMeshNamed(meshpath))
	{
		Ptr<MeshResource> nMesh = MeshResource::Create();

        Util::String fileExtension = meshpath.GetFileExtension();

        if(fileExtension == "mesh")
        {
            nMesh->loadMeshFromFile(meshpath.AsCharPtr());
        }
        else
        {
			nMesh->loadMesh(meshpath.AsCharPtr());
        }
		this->meshes.Add(meshpath, nMesh);

		return nMesh;
	}
	else
	{
		//Model is already loaded so we can just return that model.
		return this->meshes[meshpath];
	}
}

bool ResourceServer::HasMeshNamed(const Util::String& nName)
{
	if (this->meshes.Contains(nName))
		return true;
	else
		return false;
}

Ptr<TextureResource> ResourceServer::LoadTexture(const Util::String& filepath)
{
	//Make sure we've not already loaded this texture
	if (!this->HasTextureNamed(filepath))
	{
		Ptr<TextureResource> texture = TextureResource::Create();
		texture->loadFromFile(filepath.AsCharPtr());
		texture->name = filepath;
		this->textures.Add(filepath, texture);

		return texture;
	}
	else
	{
		//texture is already loaded so we can just return that texture.
		return this->textures[filepath];
	}
}


bool ResourceServer::HasTextureNamed(const Util::String& nName)
{
	if (this->textures.Contains(nName))
		return true;
	else
		return false;
}

Ptr<Material> ResourceServer::GetMaterial(const Util::String& name)
{
	if (this->HasMaterialNamed(name.AsCharPtr()))
	{
		//HACK: We shouldn't need to cast here
		return this->materials[name];
	}
	else
	{
		printf("ERROR: No such material!");
		return nullptr;
	}
}

bool ResourceServer::SetupMaterials(const Util::String& fileName)
{
	//TODO: should this really be here?

	tinyxml2::XMLDocument data;
	int result = data.LoadFile(fileName.AsCharPtr());
	

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

	while (material != nullptr)
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
			Ptr<Material> mat = Material::Create();

			// Set name
			mat->SetName(nameAttr->Value());

			// Get which frame pass we render in
			const tinyxml2::XMLElement* pass = material->FirstChildElement("Pass");
			while (pass != nullptr)
			{
				mat->SetFramePass(pass->FindAttribute("name")->Value(), pass->FindAttribute("shader")->Value());
				pass = pass->NextSiblingElement("Pass");				
			}

			// Load shader
			//mat->SetShader(pass->FindAttribute("shader")->Value());

			// Get all parameters that this material contains
			const tinyxml2::XMLElement* param = material->FirstChildElement("Param");
			while (param != nullptr)
			{
				Util::String type = param->FindAttribute("type")->Value();
								
				Util::Variable var = Util::Variable(Util::Variable::StringToType(type), Util::String(param->FindAttribute("defaultValue")->Value()));
				
				mat->AddParameter(param->FindAttribute("name")->Value(), var);
				
				param = param->NextSiblingElement("Param");
				
			}
			
			this->materials.Add(nameAttr->Value(), mat);
		}

		material = material->NextSiblingElement();
	}

	return true;
}

bool ResourceServer::HasMaterialNamed(const Util::String &nName)
{
	if (this->materials.Contains(nName))
		return true;
	else
		return false;
}

Ptr<Surface> ResourceServer::LoadSurface(const Util::String& filepath)
{
    if(this->HasSurfaceNamed(filepath))
        return this->surfaces[filepath];

    tinyxml2::XMLDocument data;
    int result = data.LoadFile(filepath.AsCharPtr());

    if (result != 0)
    {
#ifdef DEBUG
		_assert2(false, "ERROR: Could not load surface file!");
#endif // DEBUG

        return false;
    }

    tinyxml2::XMLElement* surface = data.RootElement()->FirstChildElement();

    const tinyxml2::XMLAttribute* materialTemplate = surface->FirstAttribute();

    Ptr<Surface> sur = nullptr;

    //First we check if the specified material is loaded.
    if (!this->HasMaterialNamed(materialTemplate->Value()))
    {
        printf("ERROR: No material with name: %s!\n", materialTemplate->Value());
        assert(false);
    }
    else // Load surface!
    {
        // get material
        Ptr<Material> mat = this->GetMaterial(materialTemplate->Value());

        // create our surface
        sur = Surface::Create();

		//Append this surface to materials surfacelist
		mat->surfaces.Append(sur);

        sur->name = filepath;
		sur->filePath = filepath;
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

		for (int i = 0; i < mat->textures.Size(); ++i)
		{
			Ptr<TextureResource> tex = mat->textures[i];
			if (sur->texturesByType.count(mat->TextureParamTypes[i]) == 0)
			{
				sur->textures.Append(tex);
				sur->texturesByType.insert(std::make_pair(mat->TextureParamTypes[i], tex));
			}
		}
    }

    return sur;
}

bool ResourceServer::HasSurfaceNamed(const Util::String &nName)
{
    if (this->surfaces.Contains(nName))
        return true;
    else
        return false;
}

Ptr<ModelInstance> ResourceServer::LoadModel(const Util::String& filepath)
{
	if (this->HasModelNamed(filepath))
		return this->modelInstances[filepath];

	tinyxml2::XMLDocument data;
	int result = data.LoadFile(filepath.AsCharPtr());

	if (result != 0)
	{
		_error("Could not load .mdl file called %s!", filepath.AsCharPtr());

		return nullptr;
	}

	tinyxml2::XMLElement* model = data.RootElement()->FirstChildElement();

	const tinyxml2::XMLAttribute* name = model->FirstAttribute();

	// create our surface
	Ptr<ModelInstance> mdl = ModelInstance::Create();

	//Append this surface to materials surfacelist
	this->modelInstances.Add(filepath, mdl);

	mdl->name = filepath.AsCharPtr();
	
	// Get all parameters that this surface overrides
	const tinyxml2::XMLElement* mesh = model->FirstChildElement("Mesh");
	
	Util::String meshName = mesh->FirstAttribute()->Value();

	mdl->mesh = this->LoadMesh(meshName);
	
	const tinyxml2::XMLElement* nodeElement = mesh->FirstChildElement("Node");

	while (nodeElement != nullptr)
	{
		ModelNode* node = new ModelNode;

		node->modelInstance = mdl.get();
		node->primitiveGroup = nodeElement->FindAttribute("primitivegroup")->IntValue();

		auto surface = this->LoadSurface(nodeElement->FindAttribute("surface")->Value());
		node->surface = surface.get();

		mdl->modelNodes.Append(node);
		surface->modelNodes.Append(node);

		nodeElement = nodeElement->NextSiblingElement("Node");
	}
	
	return mdl;
}

bool ResourceServer::HasModelNamed(const Util::String &nName)
{
	if (this->surfaces.Contains(nName))
		return true;
	else
		return false;
}

Util::Array<Ptr<Render::Surface>> ResourceServer::LoadMTLFile(const char* filepath)
{
	//Load surfaces/textures/materials
	FILE * file = fopen(filepath, "r");
	if (file == NULL){
		printf("ERROR: Cannot load .mtl scene!\n");
		assert(false);
		fclose(file);
	}

	Util::Array<Ptr<Render::Surface>> surfaces;

	const std::string directory = "resources/surfaces/sponza/";

	std::string surfaceName;

	//Loop through the .mtl file and load surfaces for everything in order
	while (true)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "newmtl") == 0)
		{
			char s[128];
			fscanf(file, "%s\n", s);
			surfaceName = s;
			//Append directory and extension.
			surfaceName = directory + surfaceName;
			surfaceName.append(".surface");
			
			surfaces.Append(LoadSurface(surfaceName.c_str()));
		}
	}

	fclose(file);

	return surfaces;
}




}

