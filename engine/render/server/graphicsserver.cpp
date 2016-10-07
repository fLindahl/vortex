#include "config.h"
#include "graphicsserver.h"
//#include "..\..\handlers\GameHandler.h"
//#include "..\..\player\RTSCamera.h"
//#include "..\..\basefeatures\BaseEntity.h"
#include "GL\glew.h"

namespace Render
{

void GraphicsServer::Render()
{
	glUseProgram(this->programID);
	glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &Camera::getInstance()->getViewMatrix()[0][0]);
	glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Camera::getInstance()->getProjectionMatrix()[0][0]);

	for (std::unordered_map<std::string, std::vector<std::vector<shared_ptr<GraphicsProperty>>>>::iterator it = sortedGraphicsProperties.begin(); it != sortedGraphicsProperties.end(); it++)
	{
		for (unsigned int i = 0; i < it->second.size(); i++)
		{
			//TODO: This should be initialized with the sortfunction and only called upon within Render() for each instanced drawcall.
			glm::mat4 matArray[128];

			//Bind the texture associated with this model
			it->second[i].front()->getMesh()->getTexture()->bindTexture(0);

			//Put all the transforms in an array
			for (unsigned int j = 0; j < it->second.at(i).size(); j++)
			{
				matArray[j] = it->second[i][j]->getModelMatrix();
			}

			//Send all the transforms to the gpu
			glUniformMatrix4fv(it->second[i].front()->getModelLocation(), it->second.at(i).size(), false, &matArray[0][0][0]);

			//Draw that shit
			meshes.find(it->first)->second->drawInstanced(it->second.at(i).size());
		}
	}
}

void GraphicsServer::InitShaders()
{
	this->programID = LoadShaders("shaders/vertexShader.gsl", "shaders/fragmentShader.gsl");
	ViewLocation = glGetUniformLocation(this->programID, "ViewMatrix");
	ProjectionLocation = glGetUniformLocation(this->programID, "ProjectionMatrix");

}

void GraphicsServer::addGraphicsProperty(std::shared_ptr<GraphicsProperty> prop)
{
	std::pair<std::shared_ptr<GraphicsProperty>, int> pair(prop, this->graphicscounter);
	this->graphicscounter++;
	this->properties.insert(pair);
}

void GraphicsServer::removeGraphicsProperty(std::shared_ptr<GraphicsProperty> prop)
{
	this->properties.erase(prop);
}

std::shared_ptr<MeshResource> GraphicsServer::LoadMesh(const char* meshpath)
{
	//Make sure we've not already loaded this model
	if (!this->HasModelNamed(meshpath))
	{
		std::shared_ptr<MeshResource> nMesh = std::make_shared<MeshResource>();
		nMesh->loadMeshFromFile(meshpath);

		_assert(nMesh.isValid(), "Could not load mesh!")


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

bool GraphicsServer::HasModelNamed(const std::string& nName)
{
	//Because unordered_map containers do not allow for duplicate keys, this means that 
	//count function actually returns 1 if an element with that key exists in the container, and zero otherwise.
	if (this->meshes.count(nName) > 0)
		return true;
	else
		return false;
}

std::shared_ptr<TextureResource> GraphicsServer::LoadTexture(const char* filepath)
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


bool GraphicsServer::HasTextureNamed(const std::string& nName)
{
	//Because unordered_map containers do not allow for duplicate keys, this means that 
	//count function actually returns 1 if an element with that key exists in the container, and zero otherwise.
	if (this->textures.count(nName) > 0)
		return true;
	else
		return false;
}

}