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
#include "GLFW/glfw3.h"
#include "render/resources/material.h"

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

	//Returns a mesh if it exists, otherwise loads it and then returns the new mesh
	std::shared_ptr<MeshResource> LoadMesh(const char* modelpath);
	bool HasMeshNamed(const std::string& nName);

	//Returns a texture if it exists, otherwise loads it and then returns the new texture
	std::shared_ptr<TextureResource> LoadTexture(const char* filepath);
	bool HasTextureNamed(const std::string& nName);

	//Loads a material .xml file and adds all materials to the list if they're not already defined
	bool LoadMaterials(const char* fileName);
	bool HasMaterialNamed(const std::string& nName);

private:
	//This contains all ModelInstances currently in-game.
	Util::Array<ModelInstance> modelInstances;

	//This contains all meshes.
	//Key must be unique to each mesh, and in this case, the key is the mesh's filename
	std::unordered_map<std::string, std::shared_ptr<MeshResource>> meshes;

	//This contains all textures.
	//Key must be unique to each texture. the key is the texture name loaded
	std::unordered_map<std::string, std::shared_ptr<TextureResource>> textures;

	//Contains all Materials.
	//Key must be unique to each Material. the key is the material name
	std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};

}