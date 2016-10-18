/**
Singleton ShaderServer

Contains all compiled shaders and is used to load, store and retrieve them.
*/

#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "render/resources/shaderobject.h"

#include <utility>
#include <map>

namespace Render
{

class ShaderServer
{
private:
	ShaderServer()
	{
	}

public:
	static ShaderServer* Instance()
	{
		static ShaderServer instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	ShaderServer(ShaderServer const&) = delete;
	void operator=(ShaderServer const&) = delete;

	GLuint LoadShader(const std::string& file);
	GLuint LoadVertexShader(const std::string& file);
	GLuint LoadFragmentShader(const std::string& file);

	std::shared_ptr<ShaderObject> LoadShaderObject(const std::string& vertFile, const std::string& fragFile);

	bool hasShaderLoaded(const std::string& nName);
			
private:
	std::string ReadFromFile(const std::string &filename);

	//This contains all shaders.
	//Key must be unique to each shader, and in this case, the key is the shader's filename
	std::unordered_map<std::string, GLuint> shaders;

	//Contains all shaderobjects currently compiled.
	//Key is a pair of vertex and fragment shaders, and value is a pointer to the shaderobject
	//TODO: this is ugly and temporary
	std::map<std::pair<GLuint, GLuint>, shared_ptr<ShaderObject> > shaderObjects;
};

}