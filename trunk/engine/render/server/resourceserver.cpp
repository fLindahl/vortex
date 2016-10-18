#include "config.h"
#include "resourceserver.h"

namespace Render
{

std::shared_ptr<MeshResource> ResourceServer::LoadMesh(const char* meshpath)
{
	//Make sure we've not already loaded this model
	if (!this->HasMeshNamed(meshpath))
	{
		std::shared_ptr<MeshResource> nMesh = std::make_shared<MeshResource>();
		nMesh->loadMeshFromFile(meshpath);

		nMesh->setupMesh();

		//_assert(nMesh.isValid(), "Could not load mesh!")

		std::pair<const char*, std::shared_ptr<MeshResource>> par(meshpath, nMesh);
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

}

