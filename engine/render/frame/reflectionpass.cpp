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
#include "render/resources/cubemapnode.h"
#include "render/server/lightserver.h"
#include "foundation/math/math.h"

namespace Render
{

#define TILE_SIZE 32

ReflectionPass::ReflectionPass() : 
	quality(HIGH)
{
	this->uniformBlock.zThickness = 3.5f;
	this->uniformBlock.jitter = 0.45f;
	this->uniformBlock.stride = 11.0f;
	this->uniformBlock.workGroups[0] = (RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % TILE_SIZE)) / TILE_SIZE;
	this->uniformBlock.workGroups[1] = (RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % TILE_SIZE)) / TILE_SIZE;
	this->uniformBlock.maxSteps = 80.0f;
	this->uniformBlock.maxDistance = 280.0f;
}

ReflectionPass::~ReflectionPass()
{

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

	// Setup compute shader programs
	this->SSRComputeProgram = ShaderServer::Instance()->LoadShader("SSR")->GetProgram();
	this->CubemapProgram = ShaderServer::Instance()->LoadShader("CubemapsOnly")->GetProgram();
	this->PCCubemapProgram = ShaderServer::Instance()->LoadShader("ParallaxCorrectedCubemaps")->GetProgram();
	
	glGenBuffers(1, this->ubo);

	FramePass::Setup();
}

void ReflectionPass::Execute()
{
	double time = glfwGetTime();
	
	//This needs to be done before rendering reflections on AMD cards as the compute shader dispatches before the renderpasses are done otherwise
	glFinish();

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SSRSettings), &uniformBlock, GL_STATIC_DRAW);
	
	//Switch quality settings
	//TODO: we should probably move this to vertex+fragmentshader to ~maybe~ gain performance but also enable older hardware to run these effects.
	switch (quality)
	{
	case Render::ReflectionPass::HIGH:
		glUseProgram(this->SSRComputeProgram);
		break;
	case Render::ReflectionPass::MEDIUM:
		glUseProgram(this->PCCubemapProgram);
		break;
	case Render::ReflectionPass::LOW:
		glUseProgram(this->CubemapProgram);
		break;
	default:
		_assert(false, "No valid reflection quality selected!");
		break;
	}

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "depthMap"), 4);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer());

	glActiveTexture(GL_TEXTURE5);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "normalMap"), 5);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer());

	glActiveTexture(GL_TEXTURE6);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "specularMap"), 6);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer());

	glActiveTexture(GL_TEXTURE7);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "colorMap"), 7);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer());

	auto cubemaps = Render::LightServer::Instance()->GetClosestCubemapToPoint(Graphics::MainCamera::Instance()->GetPosition());
	if (cubemaps.Size() > 0)
	{
		int numCubeMaps = Math::min(cubemaps.Size(), size_t(4));

		glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "NumCubemaps"), numCubeMaps);

		float blendFactor;

		for (int i = 0; i < 4; ++i)
		{
			std::string samplerUniformName;
			std::string blendFactorUniformName;
			if (i == 0) { samplerUniformName = VORTEX_SEMANTIC_CUBEMAP1; blendFactorUniformName = VORTEX_SEMANTIC_CUBEMAP_BLENDFACTOR1; }
			else if (i == 1) { samplerUniformName = VORTEX_SEMANTIC_CUBEMAP2; blendFactorUniformName = VORTEX_SEMANTIC_CUBEMAP_BLENDFACTOR2; }
			else if (i == 2) { samplerUniformName = VORTEX_SEMANTIC_CUBEMAP3; blendFactorUniformName = VORTEX_SEMANTIC_CUBEMAP_BLENDFACTOR3; }
			else if (i == 3) { samplerUniformName = VORTEX_SEMANTIC_CUBEMAP4; blendFactorUniformName = VORTEX_SEMANTIC_CUBEMAP_BLENDFACTOR4; }

			if (i < numCubeMaps)
			{
				glActiveTexture(GL_TEXTURE8 + i);
				glUniform1i(glGetUniformLocation(this->SSRComputeProgram, samplerUniformName.c_str()), 8 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaps[i]->GetCubeMap());
				blendFactor = cubemaps[i]->GetBlendFactor();
			}
			else
			{
				blendFactor = 0.0f;
			}

			glUniform1f(glGetUniformLocation(this->SSRComputeProgram, blendFactorUniformName.c_str()), blendFactor);
		}
	}

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

	glFinish();
	
	double time1 = glfwGetTime();
	
	double elapsedTime = time1 - time;
	
	//printf("Elapsed time for reflections: %f\n\n\n\n\n\n\n\n\n\n\n", elapsedTime);

	FramePass::Execute();
}

void ReflectionPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->reflectionBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	this->uniformBlock.workGroups[0] = (newRes.x + (newRes.x % TILE_SIZE)) / TILE_SIZE;
	this->uniformBlock.workGroups[1] = (newRes.y + (newRes.y % TILE_SIZE)) / TILE_SIZE;

	glBindTexture(GL_TEXTURE_2D, 0);
}


}