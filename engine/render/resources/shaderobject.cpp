#include "config.h"
#include "shaderobject.h"
#include <fstream>
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

	void ShaderObject::setModelMatrix(Math::Matrix4 &model)
	{
		GLuint matLoc = glGetUniformLocation(this->program, VORTEX_SEMANTIC_MODEL);
		glUniformMatrix4fv(matLoc, 1, true, (GLfloat*)model.get());
	}

	void ShaderObject::setUniMatrix4fv(Math::Matrix4 &mat4, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniformMatrix4fv(matLoc, 1, true, (GLfloat*)mat4.get());
	}

	void ShaderObject::setUniMatrix3fv(Math::Matrix3 &mat3, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniformMatrix3fv(matLoc, 1, true, (GLfloat*)mat3.get());
	}

	void ShaderObject::setUniVector4fv(Math::Vector4 &vec4, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniform4fv(matLoc, 1, (GLfloat*)vec4.get());
	}

	void ShaderObject::setUniVector3fv(Math::Vector3 &vec3, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniform3fv(matLoc, 1, (GLfloat*)vec3.get());
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

		GraphicsServer::Instance()->addShaderObject(this);
	}

	void ShaderObject::UseProgram()
	{
		glUseProgram(this->program);
	}

	GLuint ShaderObject::GetProgram()
	{
		return this->program;
	}
	
}