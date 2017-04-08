#include <config.h>
#include "shaderserver.h"
#include <sstream>
#include <fstream>
#include <string>
#include "shaderconstants.h"
#include "tinyxml2.h"
#include "render/resources/shaderobject.h"

namespace Render
{
ShaderServer::ShaderServer()
{

}

//----------------------------------
/**
	Reads a shaderfile and returns it's content as a string.
*/
Util::String ShaderServer::ReadFromFile(const Util::String &filename)
{
	Util::String content;
	std::ifstream fileStream(filename.AsCharPtr(), std::ios::in);

	if (!fileStream.is_open())
	{
		_error("[SHADER LOAD ERROR]: Could not load file from directory.");
		return "";
	}

	//TODO: We shouldn't need to use std::string here.
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.Append(line.c_str() + Util::String("\n"));
	}

	content.Append("\0");
	fileStream.close();
	return content;
}

//--------------------------------------------
/**
*/
Ptr<ShaderObject> ShaderServer::LoadShader(const Util::String& shader)
{
	if (this->HasShaderNamed(shader))
	{
		return this->shaderObjects[shader];
	}
	else
	{
		_error("[SHADER LOAD ERROR]: Could not retrieve shader!");
		_assert(false);
		return nullptr;
	}
}

//-------------------------------------
/**
	Sets up shaders based on a shader object declaration file.
	NOTE: This should only be done once per file, and this is fairly slow to do so it's better to do this before running the actual application
*/
bool ShaderServer::SetupShaders(const Util::String& file)
{
	tinyxml2::XMLDocument data;
	int result = data.LoadFile(file.AsCharPtr());
	
	if (result != 0)
	{
		_error("Could not load shaders file!");

#ifdef DEBUG
		_assert(false);
#endif // DEBUG		

		return false;
	}

	tinyxml2::XMLElement* shaders = data.RootElement()->FirstChildElement();
	tinyxml2::XMLElement* shader = shaders->FirstChildElement();

	//Faster insertion
	this->shaderObjects.BeginBulkAdd();

	while (shader != nullptr)
	{
		//First we check if the specified material is already loaded.
		const tinyxml2::XMLAttribute* nameAttr = shader->FirstAttribute();

		Util::String name = nameAttr->Value();

		if (this->HasShaderNamed(nameAttr->Value()))
		{
			_warning("Duplicate shader loaded: \" %s \". Using previously loaded shader...", nameAttr->Value());
		}
		else // Load shader!
		{
			// Create our shader
			Ptr<ShaderObject> shd = ShaderObject::Create();

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

			this->shaderObjects.Add(nameAttr->Value(), shd);
		}

		shader = shader->NextSiblingElement();		
	}

	this->shaderObjects.EndBulkAdd();

	return true;
}

//---------------------------------------------
/**
	Loads a renderstate file and adds it to the gobal renderstate list.
	TODO: This really needs some serious cleanup...
*/
RenderState ShaderServer::LoadRenderState(const Util::String& file)
{
	//Make sure we've not already loaded this render state
	if (!this->HasRenderStateLoaded(file))
	{
		if (!file.CheckFileExtension("state"))
		{
			_error("[SHADER LOAD ERROR]: File is not a .state file!");
			_assert(false);
			return RenderState();
		}

		RenderState state;

		std::ifstream fin(file.AsCharPtr());
		std::string line;
		std::istringstream sin;
		
		std::string sVal;
		float fVal;

		while (std::getline(fin, line))
		{
			sin.str(line.substr(line.find("=") + 1));
			if (line.find("CullFace ") != std::string::npos) 
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in CullFace value!\n"); }

				state.cullface = glVal;
			}
			else if (line.find("FrontFace ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "cw") { glVal = GL_CW; }
				else if (sVal == "ccw") { glVal = GL_CCW; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in FrontFace value!\n"); }

				state.frontface = glVal;
			}
			else if (line.find("CullMode ") != std::string::npos) 
			{
				sin >> sVal; 
				GLenum glVal;

				if (sVal == "back") { glVal = GL_BACK; }
				else if (sVal == "front") { glVal = GL_FRONT; }
				else if (sVal == "front_and_back") { glVal = GL_FRONT_AND_BACK; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in CullMode value!\n"); }

				state.cullmode = glVal;
			}
			else if (line.find("Blend ") != std::string::npos) 
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in Blend value!\n"); }

				state.blend = glVal;
			}
			else if (line.find("BlendSourceFunc ") != std::string::npos) 
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "zero") { glVal = GL_ZERO; }
				else if (sVal == "one") { glVal = GL_ONE; }
				else if (sVal == "src_color") { glVal = GL_SRC_COLOR; }
				else if (sVal == "one_minus_src_color") { glVal = GL_ONE_MINUS_SRC_COLOR; }
				else if (sVal == "dst_color") { glVal = GL_DST_COLOR; }
				else if (sVal == "one_minus_dst_color") { glVal = GL_ONE_MINUS_DST_COLOR; }
				else if (sVal == "src_alpha") { glVal = GL_SRC_ALPHA; }
				else if (sVal == "one_minus_src_alpha") { glVal = GL_ONE_MINUS_SRC_ALPHA; }
				else if (sVal == "dst_alpha") { glVal = GL_DST_ALPHA; }
				else if (sVal == "one_minus_dst_alpha") { glVal = GL_ONE_MINUS_DST_ALPHA; }
				else if (sVal == "constant_color") { glVal = GL_CONSTANT_COLOR; }
				else if (sVal == "one_minus_constant_color") { glVal = GL_ONE_MINUS_CONSTANT_COLOR; }
				else if (sVal == "constant_alpha") { glVal = GL_CONSTANT_ALPHA; }
				else if (sVal == "one_minus_constant_alpha") { glVal = GL_ONE_MINUS_CONSTANT_ALPHA; }
				else if (sVal == "src_alpha_saturate") { glVal = GL_SRC_ALPHA_SATURATE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in BlendSourceFunc value!\n"); }

				state.blendsourcefunc = glVal;
			}
			else if (line.find("BlendDestinationFunc ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "zero") { glVal = GL_ZERO; }
				else if (sVal == "one") { glVal = GL_ONE; }
				else if (sVal == "src_color") { glVal = GL_SRC_COLOR; }
				else if (sVal == "one_minus_src_color") { glVal = GL_ONE_MINUS_SRC_COLOR; }
				else if (sVal == "dst_color") { glVal = GL_DST_COLOR; }
				else if (sVal == "one_minus_dst_color") { glVal = GL_ONE_MINUS_DST_COLOR; }
				else if (sVal == "src_alpha") { glVal = GL_SRC_ALPHA; }
				else if (sVal == "one_minus_src_alpha") { glVal = GL_ONE_MINUS_SRC_ALPHA; }
				else if (sVal == "dst_alpha") { glVal = GL_DST_ALPHA; }
				else if (sVal == "one_minus_dst_alpha") { glVal = GL_ONE_MINUS_DST_ALPHA; }
				else if (sVal == "constant_color") { glVal = GL_CONSTANT_COLOR; }
				else if (sVal == "one_minus_constant_color") { glVal = GL_ONE_MINUS_CONSTANT_COLOR; }
				else if (sVal == "constant_alpha") { glVal = GL_CONSTANT_ALPHA; }
				else if (sVal == "one_minus_constant_alpha") { glVal = GL_ONE_MINUS_CONSTANT_ALPHA; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in BlendSourceFunc value!\n"); }

				state.blenddestinationfunc = glVal;
			}
			else if (line.find("AlphaTest ") != std::string::npos) 
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in AlphaTest value!\n"); }

				state.alphatest = glVal;
			}
			else if (line.find("AlphaFunc ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "never") { glVal = GL_NEVER; }
				else if (sVal == "less") { glVal = GL_LESS; }
				else if (sVal == "equal") { glVal = GL_EQUAL; }
				else if (sVal == "lequal") { glVal = GL_LEQUAL; }
				else if (sVal == "greater") { glVal = GL_GREATER; }
				else if (sVal == "notequal") { glVal = GL_NOTEQUAL; }
				else if (sVal == "gequal") { glVal = GL_GEQUAL; }
				else if (sVal == "always") { glVal = GL_ALWAYS; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in AlphaFunc value!\n"); }

				state.alphafunc = glVal; 
			}
			else if (line.find("AlphaClamp ") != std::string::npos) 
			{
				sin >> fVal;
				_assert2((fVal > 0.0f || fVal < 1.0f), "[SHADER LOAD ERROR]: [STATE LOAD]: AlphaClamp value can not be less than 0.0 or greater than 1.0!\n");
				state.alphaclamp = fVal;
			}
			else if (line.find("DepthTest ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in DepthTest value!\n"); }

				state.depthtest = glVal; 
			}
			else if (line.find("DepthFunc ") != std::string::npos) 
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "never") { glVal = GL_NEVER; }
				else if (sVal == "less") { glVal = GL_LESS; }
				else if (sVal == "equal") { glVal = GL_EQUAL; }
				else if (sVal == "lequal") { glVal = GL_LEQUAL; }
				else if (sVal == "greater") { glVal = GL_GREATER; }
				else if (sVal == "notequal") { glVal = GL_NOTEQUAL; }
				else if (sVal == "gequal") { glVal = GL_GEQUAL; }
				else if (sVal == "always") { glVal = GL_ALWAYS; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in DepthFunc value!\n"); }

				state.depthfunc = glVal; 
			}
			else if (line.find("DepthWrite ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }				
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in DepthWrite value!\n"); }

				state.depthwrite = glVal;
			}

			sin.clear();
		}

		this->renderStates.Add(file, state);
		return state;
	}
	else
	{
		//Return existing render state from list
		return this->renderStates[file];
	}
}

bool ShaderServer::HasRenderStateLoaded(const Util::String& nName)
{
	return this->renderStates.Contains(nName);
}

GLuint ShaderServer::LoadVertexShader(const Util::String& file)
{
	//Make sure we've not already loaded this shader
	if (!this->HasShaderProgramLoaded(file))
	{
		if (!file.CheckFileExtension("vert"))
		{
			_error("[SHADER LOAD ERROR]: File is not a .vert file!");
			return false;
		}

		Util::String content = ReadFromFile(file);
		if (content.IsEmpty())
		{
			return false;
		}

		//Attach Header
		content = VertexShaderHeader + content;
		content = ShaderHeader + content;

		const char* vs = content.AsCharPtr();

		// setup vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = (GLint)content.Length();
		glShaderSource(vertexShader, 1, &vs, &length);
		glCompileShader(vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
			_error("[VERTEX SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;

		#ifdef _DEBUG
			_assert(false);
		#endif
		}

		//Insert to our shader list
		this->shaders.Add(file, vertexShader);

		return vertexShader;

	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaders[file];
	}
}

GLuint ShaderServer::LoadFragmentShader(const Util::String& file)
{
	//Make sure we've not already loaded this shader
	if (!this->HasShaderProgramLoaded(file))
	{
		if (!file.CheckFileExtension("frag"))
		{
			_error("[SHADER LOAD ERROR]: File is not a .frag file!");
			return false;
		}

		Util::String content = ReadFromFile(file);
		if (content.IsEmpty())
		{
			return false;
		}

		//Attach Header
		content = FragmentShaderHeader + content;
		content = ShaderHeader + content;

		const char* fs = content.AsCharPtr();
	
		// setup fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLint length = (GLint)content.Length();
		glShaderSource(fragmentShader, 1, &fs, &length);
		glCompileShader(fragmentShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(fragmentShader, shaderLogSize, NULL, buf);
			_error("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;

		#ifdef _DEBUG
			_assert(false);
		#endif
		}

		//Insert to our shader list
		this->shaders.Add(file, fragmentShader);

		return fragmentShader;
	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaders[file];
	}
}

GLuint ShaderServer::LoadComputeShader(const Util::String& file)
{
	//Make sure we've not already loaded this shader
	if (!this->HasShaderProgramLoaded(file))
	{
		if (!file.CheckFileExtension("comp"))
		{
			_error("[COMPUTE SHADER LOAD ERROR]: File is not a .comp file!");
			return false;
		}

		Util::String content = ReadFromFile(file);
		if (content.IsEmpty())
		{
			return false;
		}

		// Attach header
		content = ShaderHeader + content;

		const char* fs = content.AsCharPtr();

		// setup compute shader
		GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
		GLint length = (GLint)content.Length();
		glShaderSource(computeShader, 1, &fs, &length);
		glCompileShader(computeShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(computeShader, shaderLogSize, NULL, buf);
			_error("[COMPUTE SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;

#ifdef _DEBUG
			_assert(false);
#endif
		}

		//Insert to our shader list
		this->shaders.Add(file, computeShader);

		return computeShader;
	}
	else
	{
		//Shader is already loaded so we can just return it.
		return this->shaders[file];
	}
}

bool ShaderServer::HasShaderProgramLoaded(const Util::String& file)
{
	return this->shaders.Contains(file);
}

bool ShaderServer::HasShaderNamed(const Util::String& nName)
{
	return this->shaderObjects.Contains(nName);
}

void ShaderServer::ReloadShaders()
{
	_printf("======================================");
	_printf("ShaderServer::ReloadShaders()");

	//First Update all the individual shaders
	for (int i = 0; i < this->shaders.Size(); ++i)
	{
		auto shader = this->shaders.KeyValuePairAtIndex(i);

		Util::String fileName = shader.Key();

		Util::String content = ReadFromFile(fileName);

		Util::String extension = fileName.GetFileExtension();

		// Attach header
		if (extension == "comp")
		{
			_printf("[COMPUTE SHADER RELOAD]: %s", fileName.AsCharPtr());
		}
		else if (extension == "vert")
		{
			_printf("[VERTEX SHADER RELOAD]: %s", fileName.AsCharPtr());
			content = VertexShaderHeader + content;
		}
		else if (extension == "frag")
		{
			_printf("[FRAGMENT SHADER RELOAD]: %s", fileName.AsCharPtr());
			content = FragmentShaderHeader + content;
		}

		content = ShaderHeader + content;

		const char* cstr = content.AsCharPtr();
		GLint length = (GLint)content.Length();
		glShaderSource(shader.Value(), 1, &cstr, &length);
		glCompileShader(shader.Value());

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(shader.Value(), GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(shader.Value(), shaderLogSize, NULL, buf);
			_error("[COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
	}

	for (int i = 0; i < this->shaderObjects.Size(); ++i)
	{
		auto pair = this->shaderObjects.KeyValuePairAtIndex(i);

		_printf("[SHADER OBJECT LINKER]: %s", pair.Key().AsCharPtr());
		pair.Value()->LinkShaders();
	}
	


}

}