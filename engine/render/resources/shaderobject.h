#pragma once
#include "GL/glew.h"
#include <string>
#include <iostream>
#include "foundation/math/matrix4.h"
#include "foundation/math/matrix3.h"
#include "foundation/math/vector4.h"
#include "render/server/graphicsserver.h"
#include "render/resources/modelinstance.h"
#include "foundation/util/array.h"
#include "render/server/shaderserver.h"
#include "foundation/util/string.h"
#include "render/server/shaderconstants.h"

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

	void setModelMatrix(Math::Matrix4 model);

	void setUniMatrix4fv(Math::Matrix4& mat4, const char* uniformName);
	void setUniMatrix3fv(Math::Matrix3& mat3, const char* uniformName);
	void setUniVector4fv(Math::Vector4& vec4, const char* uniformName);
	void setUniVector3fv(Math::Vector3& vec3, const char* uniformName);

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