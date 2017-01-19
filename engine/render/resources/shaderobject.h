#pragma once
#include "GL/glew.h"
#include <string>
#include <iostream>
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include "render/resources/modelinstance.h"
#include "foundation/util/array.h"
#include "render/server/shaderserver.h"
#include "render/server/shaderconstants.h"
#include "foundation/util/string.h"

namespace Render
{

class ShaderObject
{
public:
	ShaderObject();
	~ShaderObject();

	void loadVertexShader(const std::string& vertFile);
	void loadFragmentShader(const std::string& fragFile);

	void SetName(const Util::String& name) { this->name = name; }
	Util::String GetName() { return this->name; }

    void setVertexShader(const GLuint& in);
    void setFragmentShader(const GLuint& in);

	void SetRenderState(const RenderState& state);

	void setModelMatrix(const Math::mat4& model);
	void setInvModelMatrix(const Math::mat4& invmodel);

	void setUniMatrix4fv(const Math::mat4& mat4, const char* uniformName);
	void setUniVector4fv(const float* vec4, const char* uniformName);
	void setUniVector3fv(const float* vec3, const char* uniformName);

	void setUni1f(const float& f, const char* uniformName);

	void LinkShaders();

	GLuint GetProgram();
	
private:
	Util::String name;

	GLuint program;

	GLuint vertexShader;
	GLuint fragmentShader;

	// TODO: Add renderstates to the rendering system
	RenderState renderState;
};

}