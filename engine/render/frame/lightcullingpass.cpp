#include "config.h"
#include "render/server/frameserver.h"
#include "lightcullingpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/server/lightserver.h"
#include "depthpass.h"

namespace Render
{

LightCullingPass::LightCullingPass()
{
	lightServer = LightServer::Instance();

	glGenBuffers(1, this->ubo);
}

LightCullingPass::~LightCullingPass()
{

}

void LightCullingPass::Execute()
{
	// Light culling compute shader
	glUseProgram(this->lightCullingProgram);

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(lightCullingProgram, "depthMap"), 4);

	uniformBlock.pointLightCount        = (GLint)LightServer::Instance()->GetNumPointLights();
	uniformBlock.spotLightCount         = (GLint)LightServer::Instance()->GetNumSpotLights();
	uniformBlock.directionalLightCount  = (GLint)LightServer::Instance()->GetNumDirectionalLights();
    uniformBlock.tileLights             = LightServer::Instance()->GetTileLights();

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 24, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBlock), &uniformBlock, GL_STATIC_DRAW);

	//glUniform1i(glGetUniformLocation(lightCullingProgram, "lightCount"), (GLuint)LightServer::Instance()->GetNumPointLights());
	//glUniform1i(glGetUniformLocation(lightCullingProgram, "lightCount"), (GLuint)LightServer::Instance()->GetNumSpotLights());
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer());

	// Bind shader storage buffer objects for the light and index buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightServer->GetPointLightBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lightServer->GetSpotLightBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, lightServer->GetDirectionalLightBuffer());

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, lightServer->GetVisiblePointLightIndicesBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, lightServer->GetVisibleSpotLightIndicesBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, lightServer->GetVisibleDirectionalLightIndicesBuffer());

	const GLint location = glGetUniformLocation(this->lightCullingProgram, "lightImage");
	if (location == -1)
	{
		printf("Could not locate uniform location for texture in Light Culling");
	}

	glUniform1i(location, 0);
	glBindImageTexture(0, this->buffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


	// Dispatch the compute shader, using the workgroup values calculated earlier
	glDispatchCompute(lightServer->GetWorkGroupsX(), lightServer->GetWorkGroupsY(), 1);


	// Unbind the depth map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);

    FramePass::Execute();
}

void LightCullingPass::Setup()
{
	// Setup light culling compute shader program
	this->lightCullingProgram = ShaderServer::Instance()->LoadShader("LightCulling")->GetProgram();

	glGenTextures(1, &this->buffer);
	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1920, 1020, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);




    FramePass::Setup();
}

}