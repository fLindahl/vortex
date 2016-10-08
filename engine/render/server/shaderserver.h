/**
Singleton ShaderServer

Contains all compiled shaders and is used to load, store and retrieve them.
*/

#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <unordered_map>

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

	GLuint LoadVertexShader(const std::string& file);
	GLuint LoadFragmentShader(const std::string& file);

	bool hasShaderLoaded(const std::string& nName);
			
private:
	std::string ReadFromFile(const std::string &filename);

	//This contains all shaders.
	//Key must be unique to each shader, and in this case, the key is the shader's filename
	std::unordered_map<std::string, GLuint> shaders;
};

}