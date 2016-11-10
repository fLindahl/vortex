#include "config.h"
#include "shaderobject.h"
#include <fstream>
#include <render/server/renderdevice.h>
#include "render/shadersemantics.h"

namespace Render
{

	ShaderObject::ShaderObject()
	{

	}

	ShaderObject::~ShaderObject()
	{

	}

	void ShaderObject::loadVertexShader(const std::string &vertFile)
	{
		this->vertexShader = ShaderServer::Instance()->LoadVertexShader(vertFile);
	}

	void ShaderObject::loadFragmentShader(const std::string &fragFile)
	{
		this->fragmentShader = ShaderServer::Instance()->LoadFragmentShader(fragFile);
	}

	void ShaderObject::setVertexShader(const GLuint& in)
	{
		this->vertexShader = in;
	}

	void ShaderObject::setFragmentShader(const GLuint &in)
	{
		this->fragmentShader = in;
	}

	void ShaderObject::SetRenderState(const RenderState& state)
	{
		this->renderState = state;
	}

	void ShaderObject::setModelMatrix(const Math::mat4& model)
	{
		//TODO: glUniform is deprecated. we should be using uniform buffer objects instead
		GLuint matLoc = glGetUniformLocation(this->program, VORTEX_SEMANTIC_MODEL);		
		glUniformMatrix4fv(matLoc, 1, false, (GLfloat*)&model.mat.m[0][0]);
	}

	void ShaderObject::setUniMatrix4fv(const Math::mat4 &mat4, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniformMatrix4fv(matLoc, 1, false, (GLfloat*)&mat4.mat.m[0][0]);
	}

	void ShaderObject::setUniVector4fv(const float* vec4, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniform4fv(matLoc, 1, (GLfloat*)vec4);
	}

	void ShaderObject::setUniVector3fv(const float* vec3, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniform3fv(matLoc, 1, (GLfloat*)vec3);
	}

	void ShaderObject::setUni1f(const float& f, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniform1f(matLoc, f);
	}

	void ShaderObject::LinkShaders()
	{
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->fragmentShader);
		glLinkProgram(this->program);
		GLint shaderLogSize;
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
	}

	GLuint ShaderObject::GetProgram()
	{
		return this->program;
	}
	
}