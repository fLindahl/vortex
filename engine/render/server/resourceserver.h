/**
Singleton ResourceServer

Contains all compiled resources and is used to load, store and retrieve them.
*/

#pragma once
#include "core/singleton.h"
#include "core/refcounted.h"
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
	__DeclareSingleton(ResourceServer)

public:
	///Returns a mesh if it exists, otherwise loads it and then returns the new mesh
	Ptr<MeshResource> LoadMesh(const Util::String& modelpath);
	bool HasMeshNamed(const Util::String& nName);

	///Returns a texture if it exists, otherwise loads it and then returns the new texture
	Ptr<TextureResource> LoadTexture(const Util::String& filepath);
	bool HasTextureNamed(const Util::String& nName);

	///Loads a material .xml file and adds all materials to the list if they're not already defined
	Ptr<Material> GetMaterial(const Util::String& name);
	bool SetupMaterials(const Util::String&fileName);
	bool HasMaterialNamed(const Util::String& nName);

	///Loads a surface .surface file and adds all files to the list if they're not already defined
	Ptr<Surface> LoadSurface(const Util::String& filepath);
	bool HasSurfaceNamed(const Util::String& nName);

	///Loads a model .mdl file and adds it to the modellist of it's not already added in which case it just returns the model
	Ptr<ModelInstance> LoadModel(const Util::String& filepath);
	bool HasModelNamed(const Util::String& nName);
	
	///Loads an MTL file and returns an array of all the surfaces that the mtl file could load.
	///Make sure to only call this once per MTL file!
	Util::Array<Ptr<Surface>> LoadMTLFile(const char* filepath);

private:
	///This contains all ModelInstances currently in-game.
	Util::Dictionary<Util::String, Ptr<ModelInstance>> modelInstances;

	///This contains all meshes.
	///Key must be unique to each mesh, and in this case, the key is the mesh's filename
	Util::Dictionary<Util::String, Ptr<MeshResource>> meshes;

	///This contains all textures.
	///Key must be unique to each texture. the key is the texture name loaded
	Util::Dictionary<Util::String, Ptr<TextureResource>> textures;

	///Contains all Materials.
	///Key must be unique to each Material. the key is the material name
	Util::Dictionary<Util::String, Ptr<Material>> materials;

	///Contains all Surfaces.
	///Key must be unique to each Surface. the key is the surface filename
	Util::Dictionary<Util::String, Ptr<Surface>> surfaces;
};

}