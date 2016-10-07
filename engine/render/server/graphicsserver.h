/**
Singleton GraphicsServer

Contains all graphical objects and is used to call their draw functions and more.
*/

#pragma once
#include "render/properties/graphicsproperty.h"
#include <unordered_map>
#include "GLFW\glfw3.h"

namespace Render
{

/// Render states are used per shaderobject
struct RenderState
{
	//GL_CULL_FACE
	GLboolean cullFaces = GL_FALSE;
	GLenum frontFace = GL_CW;
	GLenum cullMode = GL_BACK;

	//GL_BLEND 
	GLboolean blend = GL_FALSE;
	GLenum blendSourceFunc = GL_ONE;
	GLenum blendDestinationFunc = GL_ZERO;

	//GL_ALPHA_TEST
	GLboolean alphaTest = GL_FALSE;
	GLenum alphaFunc = GL_ALWAYS;
	GLclampf alphaClamp = 0.0f;

	//GL_DEPTH_TEST
	GLboolean depthTest = GL_FALSE;
	GLenum depthFunc = GL_LESS;
};


class GraphicsServer
{
private:
	GraphicsServer()
	{
	}

public:
	static GraphicsServer* getInstance()
	{
		static GraphicsServer instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	GraphicsServer(GraphicsServer const&) = delete;
	void operator=(GraphicsServer const&) = delete;

	//This renders all graphicsproperties.
	void Render();

	//Adds the graphicsproperty to the array to be rendered.
	void addGraphicsProperty(std::shared_ptr<GraphicsProperty> prop);
	//Always call this before removing a graphicsproperty!!!
	void removeGraphicsProperty(std::shared_ptr<GraphicsProperty> prop);

	std::shared_ptr<MeshResource> LoadMesh(const char* modelpath);
	bool HasModelNamed(const std::string& nName);

	std::shared_ptr<TextureResource> LoadTexture(const char* filepath);
	bool HasTextureNamed(const std::string& nName);

	void SortProperties();
	void InitShaders();

	GLuint programID;
	GLuint ViewLocation;
	GLuint ProjectionLocation;


	GLuint textShaderID;

private:
	//TODO: Implement this.
	//Contains all shaders.
	//Key is shader's filename
	//std::unordered_map<std::string, std::shared_ptr<ShaderObject>> shaders;

	//This contains all models.
	//Key must be unique to each model, and in this case, the key is the mesh's filename
	std::unordered_map<std::string, std::shared_ptr<MeshResource>> meshes;

	//This contains all textures.
	//Key must be unique to each texture. the key is the texture name loaded
	std::unordered_map<std::string, std::shared_ptr<TextureResource>> textures;

	//This contains all graphicsproperties currently in-game.
	std::unordered_map<std::shared_ptr<GraphicsProperty>, int> properties;

	//for unique ids
	uint graphicscounter = 0;
};

}