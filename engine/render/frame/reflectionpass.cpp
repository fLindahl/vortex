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
#include "render/server/resourceserver.h"
#include "render/resources/cubemapnode.h"
#include "render/resources/geometryproxy.h"

namespace Render
{

__ImplementClass(Render::ReflectionPass, 'REFP', Render::FramePass);

#define TILE_SIZE 32

ReflectionPass::ReflectionPass() : 
	quality(ULTRA)
{
	this->uniformBlock.zThickness = 2.5f;
	this->uniformBlock.jitter = 0.225f;
	this->uniformBlock.stride = 4.0f;
	this->uniformBlock.workGroups[0] = (RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % TILE_SIZE)) / TILE_SIZE;
	this->uniformBlock.workGroups[1] = (RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % TILE_SIZE)) / TILE_SIZE;
	this->uniformBlock.maxSteps = 125.0f;
	this->uniformBlock.maxDistance = 10000.0f;

	this->cubemapData.Fill(0, 8, CubemapData());
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

	glGenTextures(1, &this->raycastBuffer);
	glBindTexture(GL_TEXTURE_2D, this->raycastBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, RenderDevice::Instance()->GetRenderResolution().x / 2, RenderDevice::Instance()->GetRenderResolution().y / 2, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Setup compute shader programs
	this->SSSRraycastpass = ShaderServer::Instance()->LoadShader("SSSR-Raycast")->GetProgram();
	this->SSSRresolvepass = ShaderServer::Instance()->LoadShader("SSSR-Resolve")->GetProgram();
	this->SSRComputeProgram = ShaderServer::Instance()->LoadShader("SSR")->GetProgram();
	this->CubemapProgram = ShaderServer::Instance()->LoadShader("CubemapsOnly")->GetProgram();
	this->PCCubemapProgram = ShaderServer::Instance()->LoadShader("ParallaxCorrectedCubemaps")->GetProgram();
	
	//Holy jesus this wall is greater than Trumps
	glUseProgram(this->SSRComputeProgram);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "depthMap"), 4);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "normalMap"), 5);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "specularMap"), 6);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, "colorMap"), 7);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[0]"), 8 + 0);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[1]"), 8 + 1);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[2]"), 8 + 2);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[3]"), 8 + 3);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[4]"), 8 + 4);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[5]"), 8 + 5);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[6]"), 8 + 6);
	glUniform1i(glGetUniformLocation(this->SSRComputeProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[7]"), 8 + 7);
	glUseProgram(this->CubemapProgram);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, "depthMap"), 4);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, "normalMap"), 5);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, "specularMap"), 6);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, "colorMap"), 7);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[0]"), 8 + 0);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[1]"), 8 + 1);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[2]"), 8 + 2);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[3]"), 8 + 3);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[4]"), 8 + 4);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[5]"), 8 + 5);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[6]"), 8 + 6);
	glUniform1i(glGetUniformLocation(this->CubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[7]"), 8 + 7);
	glUseProgram(this->PCCubemapProgram);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, "depthMap"), 4);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, "normalMap"), 5);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, "specularMap"), 6);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, "colorMap"), 7);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[0]"), 8 + 0);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[1]"), 8 + 1);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[2]"), 8 + 2);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[3]"), 8 + 3);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[4]"), 8 + 4);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[5]"), 8 + 5);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[6]"), 8 + 6);
	glUniform1i(glGetUniformLocation(this->PCCubemapProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[7]"), 8 + 7);
	glUseProgram(this->SSSRresolvepass);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, "depthMap"), 4);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, "normalMap"), 5);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, "specularMap"), 6);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, "colorMap"), 7);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[0]"), 8 + 0);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[1]"), 8 + 1);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[2]"), 8 + 2);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[3]"), 8 + 3);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[4]"), 8 + 4);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[5]"), 8 + 5);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[6]"), 8 + 6);
	glUniform1i(glGetUniformLocation(this->SSSRresolvepass, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[7]"), 8 + 7);

	
	glGenBuffers(1, this->ubo);
	glGenBuffers(1, this->cubemapUBO);

	// Create a query object.
	glGenQueries(1, queries);

	FramePass::Setup();
}

void ReflectionPass::Execute()
{
	double time = glfwGetTime();

	GLuint64 timeElapsed = 0;
	// Query current timestamp 1
	glBeginQuery(GL_TIME_ELAPSED, queries[0]);
	
	//This needs to be done before rendering reflections on AMD cards as the compute shader dispatches before the other renderpasses are done
	//glFinish();

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SSRSettings), &uniformBlock, GL_STATIC_DRAW);
	
	//Switch quality settings
	//TODO: we should probably move this to vertex+fragmentshader to ~maybe~ gain performance but also enable older hardware to run these effects.
	GLuint currentProgram;
	switch (quality)
	{
	case Render::ReflectionPass::ULTRA:
	{
		//Special case pass
		glUseProgram(this->SSSRraycastpass);
		glUniform1i(glGetUniformLocation(SSSRraycastpass, "depthMap"), 4);
		glUniform1i(glGetUniformLocation(SSSRraycastpass, "normalMap"), 5);
		glUniform1i(glGetUniformLocation(SSSRraycastpass, "specularMap"), 6);
		glUniform1i(glGetUniformLocation(SSSRraycastpass, "colorMap"), 7);
		glUniform1i(glGetUniformLocation(SSSRraycastpass, "noiseMap"), 16);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer());
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer());
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer());
		glActiveTexture(GL_TEXTURE16);
		GLuint t = ResourceServer::Instance()->LoadTexture("resources/textures/noise.tga")->GetHandle();
		glBindTexture(GL_TEXTURE_2D, ResourceServer::Instance()->LoadTexture("resources/textures/noise.tga")->GetHandle());

		Render::Resolution raycastRes = Render::RenderDevice::Instance()->GetRenderResolution();
		raycastRes.x /= 2;
		raycastRes.y /= 2;

		glUniform2i(glGetUniformLocation(SSSRraycastpass, "RayCastSize"), raycastRes.x, raycastRes.y);
		glUniform1f(glGetUniformLocation(SSSRraycastpass, "bias"), 0.7f);

		const GLint location = glGetUniformLocation(SSSRraycastpass, "rays");
		if (location == -1){
			printf("Could not locate uniform location for texture in SSRComputeProgram");
		}
		glUniform1i(location, 0);
		glBindImageTexture(0, this->raycastBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		// Dispatch the compute shader, using the workgroup values calculated earlier
		// TODO: This shouldnt be lightserver work groups.
		int wgx = (raycastRes.x + (raycastRes.x % TILE_SIZE)) / TILE_SIZE;
		int wgy = (raycastRes.y + (raycastRes.y % TILE_SIZE)) / TILE_SIZE;

		glDispatchCompute(wgx, wgy, 1);

		glUseProgram(this->SSSRresolvepass);
		currentProgram = this->SSSRresolvepass;

		glUniform1i(glGetUniformLocation(currentProgram, "noiseMap"), 16);
		glActiveTexture(GL_TEXTURE16);
		glBindTexture(GL_TEXTURE_2D, ResourceServer::Instance()->LoadTexture("resources/textures/noise.tga")->GetHandle());

		glUniform1i(glGetUniformLocation(currentProgram, "rayMap"), 17);
		glActiveTexture(GL_TEXTURE17);
		glBindTexture(GL_TEXTURE_2D, this->raycastBuffer);

		glUniform2i(glGetUniformLocation(currentProgram, "ResolveSize"), raycastRes.x, raycastRes.y);
		glUniform1f(glGetUniformLocation(currentProgram, "bias"), 0.7f);

		break;
	}
	case Render::ReflectionPass::HIGH:
		glUseProgram(this->SSRComputeProgram);
		currentProgram = this->SSRComputeProgram;
		break;
	case Render::ReflectionPass::MEDIUM:
		glUseProgram(this->PCCubemapProgram);
		currentProgram = this->PCCubemapProgram;
		break;
	case Render::ReflectionPass::LOW:
		glUseProgram(this->CubemapProgram);
		currentProgram = this->CubemapProgram;
		break;
	default:
		_assert2(false, "No valid reflection quality selected!");
		break;
	}

	//TEMPORARY for reloading realtime
	glUniform1i(glGetUniformLocation(currentProgram, "depthMap"), 4);
	glUniform1i(glGetUniformLocation(currentProgram, "normalMap"), 5);
	glUniform1i(glGetUniformLocation(currentProgram, "specularMap"), 6);
	glUniform1i(glGetUniformLocation(currentProgram, "colorMap"), 7);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[0]"), 8 + 0);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[1]"), 8 + 1);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[2]"), 8 + 2);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[3]"), 8 + 3);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[4]"), 8 + 4);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[5]"), 8 + 5);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[6]"), 8 + 6);
	glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_CUBEMAP_ARRAY "[7]"), 8 + 7);

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer());

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer());

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer());

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer());

	auto cubemaps = Render::LightServer::Instance()->GetClosestCubemapToPoint(Graphics::MainCamera::Instance()->GetPosition());
	//Limit to max 8 cubemaps
	this->numCubemaps = Math::min(cubemaps.Size(), size_t(8));
	
	if (this->numCubemaps > 0)
	{
		for (int i = 0; i < this->numCubemaps; ++i)
		{
			glActiveTexture(GL_TEXTURE8 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaps[i]->GetCubeMap());
			this->cubemapData[i].blendfactor = cubemaps[i]->GetBlendFactor();
			this->cubemapData[i].cubemapposition = cubemaps[i]->GetPosition();
			if (cubemaps[i]->GetGeometryProxy() != nullptr)
			{
				this->cubemapData[i].geometryproxy = cubemaps[i]->GetGeometryProxy()->Transform();
			}
		}

		glBindBuffer(GL_UNIFORM_BUFFER, this->cubemapUBO[0]);
		glBindBufferBase(GL_UNIFORM_BUFFER, 10, this->cubemapUBO[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CubemapData) * 8, &cubemapData[0], GL_DYNAMIC_DRAW);
	}

	//Update number of cubemaps
	glUniform1ui(glGetUniformLocation(currentProgram, "numCubemaps"), numCubemaps);
	
	const GLint location = glGetUniformLocation(currentProgram, "reflectionImage");
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

	//glFinish();
	
	//double time1 = glfwGetTime();
	
	//double elapsedTime = time1 - time;
	
	//printf("Elapsed time for reflections: %f\n\n\n\n\n\n\n\n\n\n\n", elapsedTime);


	glEndQuery(GL_TIME_ELAPSED);
	// See how much time the rendering of object i took in nanoseconds.
	glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &timeElapsed);


	printf("Elapsed time for reflections: %f\n\n\n\n\n\n\n\n\n\n\n", (float)timeElapsed * 0.000001f);

	FramePass::Execute();
}

void ReflectionPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->reflectionBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->raycastBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x/2, newRes.y/2, 0, GL_RGBA, GL_FLOAT, NULL);

	this->uniformBlock.workGroups[0] = (newRes.x + (newRes.x % TILE_SIZE)) / TILE_SIZE;
	this->uniformBlock.workGroups[1] = (newRes.y + (newRes.y % TILE_SIZE)) / TILE_SIZE;

	glBindTexture(GL_TEXTURE_2D, 0);
}


}