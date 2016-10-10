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

namespace Render
{

class ShaderObject
{
public:

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

	ShaderObject();
	~ShaderObject();

	void loadVertexShader(const std::string& vertFile);
	void loadFragmentShader(const std::string& fragFile);

	void setModelMatrix(Math::Matrix4 model);

	void setUniMatrix4fv(Math::Matrix4& mat4, const char* uniformName);
	void setUniMatrix3fv(Math::Matrix3& mat3, const char* uniformName);
	void setUniVector4fv(Math::Vector4& vec4, const char* uniformName);
	void setUniVector3fv(Math::Vector3& vec3, const char* uniformName);

	void setUni1f(const float& f, const char* uniformName);

	void LinkShaders();

	void UseProgram();

	GLuint GetProgram();

	Util::Array<ModelInstance*>& getModelInstances() { return this->modelInstances; }

private:
	GLuint program;

	GLuint vertexShader;
	GLuint fragmentShader;

	RenderState renderState;

	Util::Array<ModelInstance*> modelInstances;
};

}