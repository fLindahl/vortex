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

	///Loads an shader XML file and sets up all programs and objects
	bool SetupShaders(const Util::String& file);

	///Retrieves a shaderobject by name. NOTE: This is not very efficient.
	Ptr<ShaderObject> LoadShader(const Util::String& shader);

	///Loads and compiles a vertex shader program and thenbinds it to the GPU.
	GLuint LoadVertexShader(const Util::String& file);
	
	///Loads and compiles a fragment shader program and thenbinds it to the GPU.
	GLuint LoadFragmentShader(const Util::String& file);
	
	///Loads and compiles a compute shader program and thenbinds it to the GPU.
	GLuint LoadComputeShader(const Util::String& file);

	///Loads renderstate from a renderstate file or from existing list of states
	Render::RenderState LoadRenderState(const Util::String& file);

	///Checks whether renderstate with name has already been loaded
	bool HasRenderStateLoaded(const Util::String& nName);

	///Checks whether vertex or fragment shader with name has already been loaded
	bool HasShaderProgramLoaded(const Util::String& nName);

	///Checks whether a shader with given name exists
	bool HasShaderNamed(const Util::String& nName);

	///Reloads all shaders from source. This might be useful for debugging.
	void ReloadShaders();

private:
	///Reads a shader file and returns it's content as a Util::String
	Util::String ReadFromFile(const Util::String &filename);

	///This contains all shaders.
	///Key must be unique to each shader, and in this case, the key is the shader's filename
	Util::Dictionary<Util::String, GLuint> shaders;

	///This contains all RenderStates.
	///Key must be unique to each state, and in this case, the key is the state's filename
	Util::Dictionary<Util::String, RenderState> renderStates;

	///Contains all shaderobjects currently compiled.
	///Key is a name of shader, and value is a pointer to the shaderobject	
	Util::Dictionary<Util::String, Ptr<ShaderObject> > shaderObjects;
};

}