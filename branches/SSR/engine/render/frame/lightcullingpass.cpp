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
	glUniform1i(glGetUniformLocation(lightCullingProgram, "lightCount"), (GLuint)LightServer::Instance()->GetNumPointLights());
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer());

	// Bind shader storage buffer objects for the light and index buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightServer->GetLightBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lightServer->GetVisibleLightIndicesBuffer());

	// Dispatch the compute shader, using the workgroup values calculated earlier
	glDispatchCompute(lightServer->GetWorkGroupsX(), lightServer->GetWorkGroupsY(), 1);

	// Unbind the depth map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

    FramePass::Execute();
}

void LightCullingPass::Setup()
{
	// Setup light culling compute shader program
	this->lightCullingProgram = ShaderServer::Instance()->LoadShader("LightCulling")->GetProgram();

    FramePass::Setup();
}

}