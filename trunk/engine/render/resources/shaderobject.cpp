#include "config.h"
#include "shaderobject.h"
#include <fstream>

namespace Render
{

	ShaderObject::ShaderObject()
	{
		
	}

	ShaderObject::~ShaderObject()
	{

	}

	std::string ShaderObject::readFromFile(const std::string &filename)
	{
		std::string content;
		std::ifstream fileStream(filename, std::ios::in);

		if (!fileStream.is_open())
		{
			printf("[SHADER LOAD ERROR]: Could not load file from directory.");
			return "";
		}


		std::string line = "";
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		content.append("\0");
		fileStream.close();
		return content;
	}
	
	bool ShaderObject::loadVertexShader(const std::string &vertFile)
	{
		if (vertFile.substr(vertFile.find_last_of(".")) != ".vert")
		{
			printf("[SHADER LOAD ERROR]: File is not a .vert file!");
			return false;
		}

		std::string content = readFromFile(vertFile);
		if (content.empty())
		{
			return false;
		}

		const GLchar* vs = content.c_str();
		// setup fragment shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = (GLint)content.length();
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
		return true;
	}

	bool ShaderObject::loadFragmentShader(const std::string &fragFile)
	{
		if (fragFile.substr(fragFile.find_last_of(".")) != ".frag")
		{
			printf("[SHADER LOAD ERROR]: File is not a .frag file!");
			return false;
		}

		std::string content = readFromFile(fragFile);
		if (content.empty())
		{
			return false;
		}
		
		const GLchar* fs = content.c_str();
		// setup fragment shader
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLint length = (GLint)content.length();
		glShaderSource(this->fragmentShader, 1, &fs, &length);
		glCompileShader(this->fragmentShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->fragmentShader, shaderLogSize, NULL, buf);
			printf("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
		return true;
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

	void ShaderObject::setUni1f(const float f, const char* uniformName)
	{
		GLuint matLoc = glGetUniformLocation(this->program, uniformName);
		glUniform1f(matLoc, f);
	}

	void ShaderObject::linkShaders()
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

	void ShaderObject::applyProgram()
	{
		glUseProgram(this->program);
	}
	
}