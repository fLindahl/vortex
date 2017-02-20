#include "config.h"
#include "shaderobject.h"
#include <fstream>
#include <render/server/renderdevice.h>
#include "render/shadersemantics.h"

namespace Render
{

	ShaderObject::ShaderObject()
	{
		this->program = glCreateProgram();
	}

	ShaderObject::~ShaderObject()
	{
		glDeleteProgram(this->program);
	}

	void ShaderObject::AddShader(const GLuint& in)
	{
		this->shaders.Append(in);
		glAttachShader(this->program, in);
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

	void ShaderObject::setInvModelMatrix(const Math::mat4& invmodel)
	{
		//TODO: glUniform is deprecated. we should be using uniform buffer objects instead
		GLuint matLoc = glGetUniformLocation(this->program, VORTEX_SEMANTIC_INVMODEL);
		glUniformMatrix4fv(matLoc, 1, false, (GLfloat*)&invmodel.mat.m[0][0]);
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
	
	void ShaderObject::EnableRenderState()
	{

		//glDepthMask(this->renderState.depthwrite);

		if (this->renderState.cullface == GL_TRUE)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(this->renderState.cullmode);
			glFrontFace(this->renderState.frontface);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		if (this->renderState.blend == GL_TRUE)
		{
			glEnable(GL_BLEND);
			glBlendFunc(this->renderState.blendsourcefunc, this->renderState.blenddestinationfunc);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		if (this->renderState.alphatest == GL_TRUE)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(this->renderState.alphafunc, this->renderState.alphaclamp);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
		}

		if (this->renderState.depthtest == GL_TRUE)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(this->renderState.depthfunc);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

	}
}