#include <config.h>
#include "shaderserver.h"
#include <fstream>
#include <string>
#include "shaderconstants.h"
#include "tinyxml2.h"
#include "render/resources/shaderobject.h"

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

std::shared_ptr<ShaderObject> ShaderServer::LoadShader(const std::string& shader)
{
	if (this->HasShaderNamed(shader))
	{
		return this->shaderObjects[shader];
	}
	else
	{
		printf("[SHADER LOAD ERROR]: Could not retrieve shader!");
		assert(false);
		return nullptr;
	}
}

bool ShaderServer::SetupShaders(const std::string& file)
{
	tinyxml2::XMLDocument data;
	int result = data.LoadFile(file.c_str());
	
	if (result != 0)
	{
		printf("ERROR: Could not load materials file!");

#ifdef DEBUG
		assert(false);
#endif // DEBUG		

		return false;
	}

	tinyxml2::XMLElement* shaders = data.RootElement()->FirstChildElement();
	tinyxml2::XMLElement* shader = shaders->FirstChildElement();

	while (true)
	{
		//First we check if the specified material is already loaded.
		const tinyxml2::XMLAttribute* nameAttr = shader->FirstAttribute();

		Util::String name = nameAttr->Value();

		if (this->HasShaderNamed(nameAttr->Value()))
		{
			printf("WARNING: Duplicate shader loaded: \" %s \". Using previously loaded shader...", nameAttr->Value());
		}
		else // Load shader!
		{
			// Create our shader
			shared_ptr<ShaderObject> shd = make_shared<ShaderObject>();

			// Set name
			shd->SetName(nameAttr->Value());

			const tinyxml2::XMLElement* vertexshader = shader->FirstChildElement("VertexShader");
			const tinyxml2::XMLElement* fragmentshader = shader->FirstChildElement("FragmentShader");
			const tinyxml2::XMLElement* renderstate = shader->FirstChildElement("RenderState");

			GLuint vert = this->LoadVertexShader(vertexshader->FirstAttribute()->Value());
			GLuint frag = this->LoadFragmentShader(fragmentshader->FirstAttribute()->Value());

			shd->setVertexShader(vert);
			shd->setFragmentShader(frag);

			Render::RenderState states = LoadRenderState(renderstate->FirstAttribute()->Value());
			shd->SetRenderState(states);

			shd->LinkShaders();

			this->shaderObjects.insert(std::make_pair(nameAttr->Value(), shd));
		}

		if (shader->NextSiblingElement() != nullptr)
		{
			shader = shader->NextSiblingElement();
		}
		else
		{
			// end of shaders
			break;
		}
	}

	return true;
}

RenderState ShaderServer::LoadRenderState(const std::string& file)
{
	//Make sure we've not already loaded this render state
	if (!this->HasRenderStateLoaded(file))
	{
		if (file.substr(file.find_last_of(".")) != ".state")
		{
			printf("[SHADER LOAD ERROR]: File is not a .state file!");
			assert(false);
			return RenderState();
		}

		std::string content = ReadFromFile(file);
		if (content.empty())
		{
			return RenderState();
		}

		//TODO: Implement loader / we should handle the state file here.
		//HACK: returning default renderstate until this has been implemented
		return RenderState();

	}
}

bool ShaderServer::HasRenderStateLoaded(const std::string& nName)
{
	return this->renderStates.count(nName);
}

GLuint ShaderServer::LoadVertexShader(const std::string& file)
{
	//Make sure we've not already loaded this shader
	if (!this->HasShaderProgramLoaded(file))
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
		this->shaderPrograms.insert(par);

		return vertexShader;

	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaderPrograms.find(file)->second;
	}
}

GLuint ShaderServer::LoadFragmentShader(const std::string& file)
{
	//Make sure we've not already loaded this shader
	if (!this->HasShaderProgramLoaded(file))
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
		this->shaderPrograms.insert(par);

		return fragmentShader;
	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaderPrograms.find(file)->second;
	}
}

bool ShaderServer::HasShaderProgramLoaded(const std::string& file)
{
	return this->shaderPrograms.count(file);
}

bool ShaderServer::HasShaderNamed(const std::string& nName)
{
	return this->shaderObjects.count(nName);
}

}