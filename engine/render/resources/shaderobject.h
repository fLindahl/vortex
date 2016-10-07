#pragma once
#include <string>
#include <iostream>
#include "GL/glew.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/matrix3.h"
#include "foundation/math/vector4.h"
#include "render/server/graphicsserver.h"

namespace Render
{

class ShaderObject
{
public:
	ShaderObject();
	~ShaderObject();

	static std::string readFromFile(const std::string &filename);
	bool loadVertexShader(const std::string &vertFile);
	bool loadFragmentShader(const std::string &fragFile);

	void setUniMatrix4fv(Math::Matrix4 &mat4, const char* uniformName);
	void setUniMatrix3fv(Math::Matrix3 &mat3, const char* uniformName);
	void setUniVector4fv(Math::Vector4 &vec4, const char* uniformName);
	void setUniVector3fv(Math::Vector3 &vec3, const char* uniformName);

	void setUni1f(const float f, const char* uniformName);

	void linkShaders();

	void applyProgram();

private:
	GLuint program;

	GLuint vertexShader;
	GLuint fragmentShader;

	RenderState renderState;
};

}