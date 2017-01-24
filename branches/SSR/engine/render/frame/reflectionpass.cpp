#include "config.h"
#include "render/server/frameserver.h"
#include "reflectionpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"

namespace Render
{

ReflectionPass::ReflectionPass()
{

}

ReflectionPass::~ReflectionPass()
{

}

void ReflectionPass::Execute()
{
	FramePass::Execute();
}

void ReflectionPass::Setup()
{
	/*
	// Setup light culling compute shader program
	this->SSRComputeProgram = glCreateProgram();
	const char filepath[] = "resources/shaders/compute/ssr.comp";
	glAttachShader(this->SSRComputeProgram, ShaderServer::Instance()->LoadComputeShader(filepath));
	glLinkProgram(this->SSRComputeProgram);
	GLint shaderLogSize;
	glGetProgramiv(this->SSRComputeProgram, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->SSRComputeProgram, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}
	*/
    FramePass::Setup();
}

}