#include "config.h"
#include "GL/glew.h"
#include "renderdevice.h"
#include "resourceserver.h"
#include "render/graphics/camera.h"
#include "render/resources/shaderobject.h"
#include "render/shadersemantics.h"
#include "render/resources/modelinstance.h"
#include "render/resources/meshresource.h"
#include "render/resources/textureresource.h"
#include "render/properties/graphicsproperty.h"
#include "foundation/util/variable.h"
#include "frameserver.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "lightserver.h"
#include "render/debugrender/debugrenderer.h"
#include "render/frame/framepass.h"
#include "render/frame/depthpass.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/frame/reflectionpass.h"
#include "render/frame/pickingpass.h"


namespace Render
{
RenderDevice::RenderDevice()
{
	currentProgram = 0;
	pickingEnabled = false;
}

void RenderDevice::Initialize()
{
	//We need an initial resolution so that we can build our framebuffers upon something.
	this->renderResolution = { 800, 600 };

	glGenBuffers(1, this->ubo);
}

void RenderDevice::SetRenderResolution(const Resolution& res)
{
	this->renderResolution = res;
	Graphics::MainCamera::Instance()->UpdateProjectionMatrix();
	FrameServer::Instance()->UpdateResolutions();
	LightServer::Instance()->UpdateWorkGroups();
	LightServer::Instance()->UpdatePointLightBuffer();
	LightServer::Instance()->UpdateSpotLightBuffer();
}

void RenderDevice::SetRenderResolution(const int& x, const int& y)
{
	SetRenderResolution({ x, y });
}

void RenderDevice::SetWindowResolution(const int& x, const int& y)
{
	this->windowResolution.x = x;
	this->windowResolution.y = y;
}

void RenderDevice::SetUniformBuffer(const Graphics::Camera* camera)
{
	//Set global matrix uniform buffer block for this frame
	uniformBufferBlock.View = camera->getView();
	uniformBufferBlock.Projection = camera->getProjection();
	uniformBufferBlock.ViewProjection = Math::mat4::multiply(camera->getView(), camera->getProjection());

	uniformBufferBlock.InvView = camera->getInvView();
	uniformBufferBlock.InvProjection = camera->getInvProjection();
	uniformBufferBlock.InvViewProjection = camera->getInvViewProjection();

	uniformBufferBlock.viewToTextureSpace = camera->getViewToTextureSpace();

	uniformBufferBlock.CameraPos = camera->GetPosition();

	uniformBufferBlock.ScreenSize = this->renderResolution;
	uniformBufferBlock.TimeAndRandom[0] = (GLfloat)glfwGetTime();

	std::srand((unsigned int)time(0));
	uniformBufferBlock.TimeAndRandom[1] = (GLfloat)std::rand();

	uniformBufferBlock.LightTileWorkGroups[0] = LightServer::Instance()->workGroupsX;
	uniformBufferBlock.LightTileWorkGroups[1] = LightServer::Instance()->workGroupsY;

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);
}

void RenderDevice::Render(bool drawToScreen)
{
	glViewport(0, 0, renderResolution.x, renderResolution.y);
	
	//----------------
	// Start rendering
	SetUniformBuffer(Graphics::MainCamera::Instance());

	for (auto pass : FrameServer::Instance()->framePasses)
	{
		pass->Execute();
	}

	// Render Debug Shapes!
	FrameServer::Instance()->GetFlatGeometryLitPass()->BindFrameBuffer();
	Debug::DebugRenderer::Instance()->DrawCommands();

	glViewport(0, 0, windowResolution.x, windowResolution.y);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK); // Set the back buffer as the draw buffer

	//Copy final colorbuffer to screen if specified
	if (drawToScreen)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FrameServer::Instance()->FlatGeometryLit->frameBufferObject);
		glBlitFramebuffer(0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->windowResolution.x, this->windowResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		//This is only for OGL 4.5 and it might cause issues with older cards...
		//glBlitNamedFramebuffer(FrameServer::Instance()->FlatGeometryLit->frameBufferObject, 0, 0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->windowResolution.x, this->windowResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);

	//change some render states so that Imgui works correctly
	glAlphaFunc(GL_ALWAYS, 0.0f);
	glDepthFunc(GL_LESS);
}

void RenderDevice::RenderToTexture(const GLuint& outFrameBuffer, const Graphics::Camera& camera)
{
	glViewport(0, 0, renderResolution.x, renderResolution.y);

	//----------------
	// Start rendering
	SetUniformBuffer(&camera);

	for (auto pass : FrameServer::Instance()->framePasses)
	{
		pass->Execute();
	}
		
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outFrameBuffer);

	//Copy final colorbuffer to screen if specified
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FrameServer::Instance()->FlatGeometryLit->frameBufferObject);
	glBlitFramebuffer(0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->renderResolution.x, this->renderResolution.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);

	glViewport(0, 0, windowResolution.x, windowResolution.y);

	//change some render states so that Imgui works correctly
	glAlphaFunc(GL_ALWAYS, 0.0f);
	glDepthFunc(GL_LESS);
}




}