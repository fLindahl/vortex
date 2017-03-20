/**
Singleton ResourceServer

Contains all compiled resources and is used to load, store and retrieve them.
*/

#pragma once
#include <memory>
#include "GL/glew.h"
#include <unordered_map>
#include "render/resources/meshresource.h"
#include "render/resources/textureresource.h"
#include "render/frame/framepass.h"
#include "GLFW/glfw3.h"
#include "render/resources/material.h"
#include "foundation/util/dictionary.h"

namespace Render
{
class ResourceServer
{
private:
	ResourceServer()
	{
	}

public:
	static ResourceServer* Instance()
	{
		static ResourceServer instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	ResourceServer(ResourceServer const&) = delete;
	void operator=(ResourceServer const&) = delete;
	
	///Returns a mesh if it exists, otherwise loads it and then returns the new mesh
	std::shared_ptr<MeshResource> LoadMesh(const Util::String& modelpath);
	bool HasMeshNamed(const Util::String& nName);

	///Returns a texture if it exists, otherwise loads it and then returns the new texture
	std::shared_ptr<TextureResource> LoadTexture(const Util::String& filepath);
	bool HasTextureNamed(const Util::String& nName);

	///Loads a material .xml file and adds all materials to the list if they're not already defined
	std::shared_ptr<Material> GetMaterial(const Util::String& name);
	bool SetupMaterials(const Util::String&fileName);
	bool HasMaterialNamed(const Util::String& nName);

	///Loads a surface .surface file and adds all files to the list if they're not already defined
	std::shared_ptr<Surface> LoadSurface(const Util::String& filepath);
	bool HasSurfaceNamed(const Util::String& nName);

	///Loads a model .mdl file and adds it to the modellist of it's not already added in which case it just returns the model
	std::shared_ptr<ModelInstance> LoadModel(const Util::String& filepath);
	bool HasModelNamed(const Util::String& nName);
	
	///Loads an MTL file and returns an array of all the surfaces that the mtl file could load.
	///Make sure to only call this once per MTL file!
	Util::Array<std::shared_ptr<Surface>> LoadMTLFile(const char* filepath);

private:
	///This contains all ModelInstances currently in-game.
	Util::Dictionary<Util::String, std::shared_ptr<ModelInstance>> modelInstances;

	///This contains all meshes.
	///Key must be unique to each mesh, and in this case, the key is the mesh's filename
	Util::Dictionary<Util::String, std::shared_ptr<MeshResource>> meshes;

	///This contains all textures.
	///Key must be unique to each texture. the key is the texture name loaded
	Util::Dictionary<Util::String, std::shared_ptr<TextureResource>> textures;

	///Contains all Materials.
	///Key must be unique to each Material. the key is the material name
	Util::Dictionary<Util::String, std::shared_ptr<Material>> materials;

	///Contains all Surfaces.
	///Key must be unique to each Surface. the key is the surface filename
	Util::Dictionary<Util::String, std::shared_ptr<Surface>> surfaces;
};

}