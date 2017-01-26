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
		printf("ERROR: Could not load shaders file!");

#ifdef DEBUG
		assert(false);
#endif // DEBUG		

		return false;
	}

	tinyxml2::XMLElement* shaders = data.RootElement()->FirstChildElement();
	tinyxml2::XMLElement* shader = shaders->FirstChildElement();

	while (shader != nullptr)
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
			std::shared_ptr<ShaderObject> shd = std::make_shared<ShaderObject>();

			// Set name
			shd->SetName(nameAttr->Value());

			const tinyxml2::XMLElement* computeshader = shader->FirstChildElement("ComputeShader");

			if (computeshader != nullptr)
			{
				//Load Compute Shader
				shd->type = ShaderObjectType::COMPUTE;
				GLuint comp = this->LoadComputeShader(computeshader->FirstAttribute()->Value());
				shd->AddShader(comp);
			}
			else // Load regular shader
			{
				shd->type = ShaderObjectType::VERTEX_FRAGMENT;

				const tinyxml2::XMLElement* vertexshader = shader->FirstChildElement("VertexShader");
				const tinyxml2::XMLElement* fragmentshader = shader->FirstChildElement("FragmentShader");
				const tinyxml2::XMLElement* renderstate = shader->FirstChildElement("RenderState");

				GLuint vert = this->LoadVertexShader(vertexshader->FirstAttribute()->Value());
				GLuint frag = this->LoadFragmentShader(fragmentshader->FirstAttribute()->Value());

				shd->AddShader(vert);
				shd->AddShader(frag);

				Render::RenderState states = LoadRenderState(renderstate->FirstAttribute()->Value());
				shd->SetRenderState(states);
			}

			shd->LinkShaders();

			this->shaderObjects.insert(std::make_pair(nameAttr->Value(), shd));
		}

		shader = shader->NextSiblingElement();		
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
		
	}

	return RenderState();

}

bool ShaderServer::HasRenderStateLoaded(const std::string& nName)
{
	if (this->renderStates.count(nName) > 0)
		return true;
	else
		return false;
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
		content = ShaderHeader + content;

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
		content = ShaderHeader + content;

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

GLuint ShaderServer::LoadComputeShader(const std::string& file)
{
	//Make sure we've not already loaded this shader
	if (!this->HasShaderProgramLoaded(file))
	{
		if (file.substr(file.find_last_of(".")) != ".comp")
		{
			printf("[COMPUTE SHADER LOAD ERROR]: File is not a .comp file!");
			return false;
		}

		std::string content = ReadFromFile(file);
		if (content.empty())
		{
			return false;
		}

		// Attach header
		content = ShaderHeader + content;

		const char* fs = content.c_str();

		// setup fragment shader
		GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
		GLint length = (GLint)content.length();
		glShaderSource(computeShader, 1, &fs, &length);
		glCompileShader(computeShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(computeShader, shaderLogSize, NULL, buf);
			printf("[COMPUTE SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;

#ifdef _DEBUG
			assert(false);
#endif
		}

		//Insert to our shader list
		std::pair<std::string, GLuint> par(file, computeShader);
		this->shaders.insert(par);

		return computeShader;
	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaders.find(file)->second;
	}
}

bool ShaderServer::HasShaderProgramLoaded(const std::string& file)
{
	if (this->shaders.count(file) > 0)
		return true;
	else
		return false;
}

bool ShaderServer::HasShaderNamed(const std::string& nName)
{
	if (this->shaderObjects.count(nName) > 0)
		return true;
	else
		return false;
}

void ShaderServer::ReloadShaders()
{
	printf("\n");
	printf("======================================\n");
	printf("ShaderServer::ReloadShaders()\n");
	printf("\n");

	//First Update all the individual shaders
	for (auto shader : this->shaders)
	{
		std::string fileName = shader.first;

		std::string content = ReadFromFile(fileName);

		std::string extension = fileName.substr(fileName.find_last_of("."));

		// Attach header
		if (extension == ".comp")
		{
			printf("[COMPUTE SHADER RELOAD]: %s\n", fileName.c_str());
		}
		else if (extension == ".vert")
		{
			printf("[VERTEX SHADER RELOAD]: %s\n", fileName.c_str());
			content = VertexShaderHeader + content;
		}
		else if (extension == ".frag")
		{
			printf("[FRAGMENT SHADER RELOAD]: %s\n", fileName.c_str());
			content = FragmentShaderHeader + content;
		}

		content = ShaderHeader + content;

		const char* cstr = content.c_str();
		GLint length = (GLint)content.length();
		glShaderSource(shader.second, 1, &cstr, &length);
		glCompileShader(shader.second);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(shader.second, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(shader.second, shaderLogSize, NULL, buf);
			printf("[COMPILE ERROR]: %s\n\n\n", buf);
			delete[] buf;
		}
	}

	printf("\n");

	for (auto pair : this->shaderObjects)
	{
		auto object = pair.second;

		printf("[SHADER OBJECT LINKER]: %s\n", pair.first.c_str());
		object->LinkShaders();
	}


}

}