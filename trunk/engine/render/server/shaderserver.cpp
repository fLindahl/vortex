#include <config.h>
#include "shaderserver.h"
#include <fstream>
#include <string>
#include "shaderconstants.h"

namespace Render
{

std::string ShaderServer::ReadFromFile(const std::string &filename)
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


GLuint ShaderServer::LoadShader(const std::string& file)
{
    //Make sure we've not already loaded this shader
    if (!this->hasShaderLoaded(file))
    {
        GLenum shaderType;
        if (file.substr(file.find_last_of(".")) == ".vert")
        {
            shaderType = GL_VERTEX_SHADER;
        }
        else if (file.substr(file.find_last_of(".")) != ".vert")
        {
            shaderType = GL_FRAGMENT_SHADER;
        }
        else
        {
            printf("[SHADER LOAD ERROR]: File is not a .vert or .frag file!");
            return false;
        }

        std::string content = ReadFromFile(file);
        if (content.empty())
        {
            return false;
        }

        //Attach Header
        if(shaderType == GL_VERTEX_SHADER)
        {
            content = VertexShaderHeader + content;
        }
        else
        {
            content = FragmentShaderHeader + content;
        }

        const char* s = content.c_str();

        // setup fragment shader
        GLuint shader = glCreateShader(shaderType);
        GLint length = (GLint)content.length();
        glShaderSource(shader, 1, &s, &length);
        glCompileShader(shader);

        // get error log
        GLint shaderLogSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLogSize);
        if (shaderLogSize > 0)
        {
            char* buf = new char[shaderLogSize];
            glGetShaderInfoLog(shader, shaderLogSize, NULL, buf);
            printf("[SHADER COMPILE ERROR]: %s", buf);
            delete[] buf;

#ifdef _DEBUG
            assert(false);
#endif
        }

        //Insert to our shader list
        std::pair<std::string, GLuint> par(file, shader);
        this->shaders.insert(par);

        return shader;
    }
    else
    {
        //Shader is already loaded so we can just return it.
        return this->shaders.find(file)->second;
    }
}

GLuint ShaderServer::LoadVertexShader(const std::string& file)
{
	//Make sure we've not already loaded this shader
	if (!this->hasShaderLoaded(file))
	{
		if (file.substr(file.find_last_of(".")) != ".vert")
		{
			printf("[SHADER LOAD ERROR]: File is not a .vert file!");
			return false;
		}

		std::string content = ReadFromFile(file);
		if (content.empty())
		{
			return false;
		}

		//Attach Header
		content = VertexShaderHeader + content;

		const char* vs = content.c_str();

		// setup fragment shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = (GLint)content.length();
		glShaderSource(vertexShader, 1, &vs, &length);
		glCompileShader(vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
			printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;

		#ifdef _DEBUG
			assert(false);
		#endif
		}

		//Insert to our shader list
		std::pair<std::string, GLuint> par(file, vertexShader);
		this->shaders.insert(par);

		return vertexShader;

	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaders.find(file)->second;
	}
}

GLuint ShaderServer::LoadFragmentShader(const std::string& file)
{
	//Make sure we've not already loaded this shader
	if (!this->hasShaderLoaded(file))
	{
		if (file.substr(file.find_last_of(".")) != ".frag")
		{
			printf("[SHADER LOAD ERROR]: File is not a .frag file!");
			return false;
		}

		std::string content = ReadFromFile(file);
		if (content.empty())
		{
			return false;
		}

		//Attach Header
		content = FragmentShaderHeader + content;

		const char* fs = content.c_str();
	
		// setup fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLint length = (GLint)content.length();
		glShaderSource(fragmentShader, 1, &fs, &length);
		glCompileShader(fragmentShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(fragmentShader, shaderLogSize, NULL, buf);
			printf("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;

		#ifdef _DEBUG
			assert(false);
		#endif
		}

		//Insert to our shader list
		std::pair<std::string, GLuint> par(file, fragmentShader);
		this->shaders.insert(par);

		return fragmentShader;
	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaders.find(file)->second;
	}
}
std::shared_ptr<ShaderObject> ShaderServer::LoadShaderObject(const std::string &vertFile, const std::string &fragFile)
{
    GLuint vert = this->LoadVertexShader(vertFile);
    GLuint frag = this->LoadFragmentShader(fragFile);

    std::pair<GLuint, GLuint> pair = std::make_pair(vert, frag);

	if (this->shaderObjects.count(pair))
	{
		std::shared_ptr<ShaderObject> shdObj = this->shaderObjects.find(pair)->second;
		return shdObj;
    }
    else
    {
        //We need to create a new shader object!
		std::shared_ptr<ShaderObject> shdObj = std::make_shared<ShaderObject>();
        shdObj->setVertexShader(vert);
        shdObj->setFragmentShader(frag);
        shdObj->LinkShaders();
        this->shaderObjects.insert(std::make_pair(pair, shdObj));
        return shdObj;
    }
}

bool ShaderServer::hasShaderLoaded(const std::string& file)
{
	return this->shaders.count(file);
}

}