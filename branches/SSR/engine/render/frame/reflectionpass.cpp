#include "config.h"
#include "render/server/frameserver.h"
#include "reflectionpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/frame/depthpass.h"
#include "render/frame/flatgeometrylitpass.h"

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
	// Set uniforms
	this->uniformBlock.zThickness = 100.0f;
	this->uniformBlock.jitter = 0.0f;
	this->uniformBlock.stride = 1.0f;
	this->uniformBlock.workGroups[0] = (RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % 16)) / 16;
	this->uniformBlock.workGroups[1] = (RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % 16)) / 16;
	this->uniformBlock.maxSteps = 64.0f;
	this->uniformBlock.maxDistance = 1000.0f;
	
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBlock), &uniformBlock, GL_STATIC_DRAW);
	
	//Compute shader
	glUseProgram(this->SSRComputeProgram);

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "depthMap"), 4);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetBuffer());

	glActiveTexture(GL_TEXTURE5);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "normalMap"), 5);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer());

	glActiveTexture(GL_TEXTURE6);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "specularMap"), 6);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer());

	glActiveTexture(GL_TEXTURE7);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "colorMap"), 7);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer());

	const GLint location = glGetUniformLocation(this->SSRComputeProgram, "reflectionImage");
	if (location == -1){
		printf("Could not locate uniform location for texture in SSRComputeProgram");
	}

	glUniform1i(location, 0);
	glBindImageTexture(0, this->reflectionBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// Dispatch the compute shader, using the workgroup values calculated earlier
	// TODO: This shouldnt be lightserver work groups.
	glDispatchCompute(this->uniformBlock.workGroups[0], this->uniformBlock.workGroups[1], 1);

	// Unbind the maps
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, 0);

	FramePass::Execute();
}

void ReflectionPass::Setup()
{
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glGenTextures(1, &this->reflectionBuffer);
	glBindTexture(GL_TEXTURE_2D, this->reflectionBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

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

	glGenBuffers(1, this->ubo);
	
    FramePass::Setup();
}

void ReflectionPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->reflectionBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}


}