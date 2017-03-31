/**
Singleton ShaderServer

Contains all compiled shaders and is used to load, store and retrieve them.
*/

#pragma once
#include "config.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shaderconstants.h"
//#include "foundation/util/string.h"
#include <utility>
#include <map>
#include <unordered_map>
#include "core/refcounted.h"
#include "core/singleton.h"

namespace Render
{

class ShaderObject;

class ShaderServer
{
	__DeclareSingleton(ShaderServer)
public:

	//Loads an XML file and sets up all programs and objects
	bool SetupShaders(const std::string& file);

	Ptr<ShaderObject> LoadShader(const std::string& shader);

	//Loads individual shader programs
	GLuint LoadVertexShader(const std::string& file);
	GLuint LoadFragmentShader(const std::string& file);
	GLuint LoadComputeShader(const std::string& file);

	//Loads renderstate from a renderstate file or from existing list of states
	Render::RenderState LoadRenderState(const std::string& file);

	//Checks whether renderstate with name has already been loaded
	bool HasRenderStateLoaded(const std::string& nName);

	//Checks whether vertex or fragment shader with name has already been loaded
	bool HasShaderProgramLoaded(const std::string& nName);

	//Checks whether a shader with given name exists
	bool HasShaderNamed(const std::string& nName);

	// Reloads all shaders from source. This might be useful for debugging.
	void ReloadShaders();

private:
	std::string ReadFromFile(const std::string &filename);

	//This contains all shaders.
	//Key must be unique to each shader, and in this case, the key is the shader's filename
	std::unordered_map<std::string, GLuint> shaders;

	//This contains all RenderStates.
	//Key must be unique to each state, and in this case, the key is the state's filename
	std::unordered_map<std::string, RenderState> renderStates;

	//Contains all shaderobjects currently compiled.
	//Key is a name of shader, and value is a pointer to the shaderobject	
	std::map<std::string, Ptr<ShaderObject> > shaderObjects;
};

}