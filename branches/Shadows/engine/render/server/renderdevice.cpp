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
#include "render/frame/shadowmap.h"

#ifdef _DEBUG
	#include "render/frame/lightdebugpass.h"
#endif

namespace Render
{
RenderDevice::RenderDevice()
{
	glGenBuffers(1, this->ubo);
	currentProgram = 0;
}

void RenderDevice::Initialize()
{
	//We need an initial resolution so that we can build our framebuffers upon something.
	this->renderResolution = { 800, 600 };
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

void RenderDevice::Render(bool drawToScreen)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, renderResolution.x, renderResolution.y);

    //Set global matrix uniform buffer block for this frame
    uniformBufferBlock.View = Graphics::MainCamera::Instance()->getView();
    uniformBufferBlock.Projection = Graphics::MainCamera::Instance()->getProjection();
	uniformBufferBlock.ViewProjection = Math::mat4::multiply(Graphics::MainCamera::Instance()->getView(), Graphics::MainCamera::Instance()->getProjection());

	uniformBufferBlock.InvView = Graphics::MainCamera::Instance()->getInvView();
	uniformBufferBlock.InvProjection = Graphics::MainCamera::Instance()->getInvProjection();
	uniformBufferBlock.InvViewProjection = Graphics::MainCamera::Instance()->getInvViewProjection();

	uniformBufferBlock.CameraPos = Graphics::MainCamera::Instance()->GetPosition();

	uniformBufferBlock.ScreenSize = this->renderResolution;
	uniformBufferBlock.TimeAndRandom[0] = (GLfloat)glfwGetTime();

	std::srand((unsigned int)time(0));
	uniformBufferBlock.TimeAndRandom[1] = (GLfloat)std::rand();

	uniformBufferBlock.LightTileWorkGroups[0] = LightServer::Instance()->workGroupsX;
	uniformBufferBlock.LightTileWorkGroups[1] = LightServer::Instance()->workGroupsY;

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);

    //HACK: Render states should be done per shaderobject (in some cases)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//----------------
	// Start rendering

	//TODO: We should iterate through every pass instead of doing it this way!

	// Depth pre-pass
	std::weak_ptr<DepthPass> dPass = FrameServer::Instance()->GetDepthPass();
	auto depthPass = dPass.lock();

    //Run depth pass
	depthPass->Execute();
	
	std::weak_ptr<FramePass>framePass = FrameServer::Instance()->GetLightCullingPass();
	auto lightCullingPass = framePass.lock();

	lightCullingPass->Execute();

#ifdef _DEBUG
	//FrameServer::Instance()->lightdebugpass->Execute();
#endif

	//Bind the final color buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, FrameServer::Instance()->finalColorFrameBufferObject);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::weak_ptr<FlatGeometryLitPass> fglp = FrameServer::Instance()->FlatGeometryLit;
	auto flatGeometryLitPass = fglp.lock();

	//Run draw pass
	flatGeometryLitPass->Execute();

	//Reflections
	//std::weak_ptr<ReflectionPass> ref = FrameServer::Instance()->reflectionPass;
	//auto reflectionPass = ref.lock();
	//reflectionPass->Execute();

	//SWARLEY
	std::weak_ptr<ShadowMap> shad = FrameServer::Instance()->shadowmap;
	auto shadowMap = shad.lock();
	shadowMap->Execute();



	//-------------------
	// Render Debug Shapes!
	flatGeometryLitPass->BindFrameBuffer();
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
}




}