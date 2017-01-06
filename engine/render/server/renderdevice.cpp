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

namespace Render
{
RenderDevice::RenderDevice()
{
	glGenBuffers(1, this->ubo);
	currentProgram = 0;
}

void RenderDevice::Initialize()
{
#ifdef _LIGHT_DEBUG
	this->lightDebugShader = std::make_shared<ShaderObject>();

	// Set name
	this->lightDebugShader->SetName("lightShader");

	GLuint vertShader = ShaderServer::Instance()->LoadVertexShader("resources/shaders/vertex/static_obj.vert");
	GLuint fragmentShader = ShaderServer::Instance()->LoadFragmentShader("resources/shaders/fragment/light_debug.frag");

	this->lightDebugShader->setVertexShader(vertShader);
	this->lightDebugShader->setFragmentShader(fragmentShader);
	this->lightDebugShader->LinkShaders();
#endif // _DEBUG
}

void RenderDevice::SetRenderResolution(const Resolution& res)
{
	this->renderResolution = res;
	Graphics::MainCamera::Instance()->UpdateProjectionMatrix();
	FrameServer::Instance()->UpdateResolutions();
	LightServer::Instance()->UpdateWorkGroups();
	LightServer::Instance()->UpdateLightBuffer();
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
    uniformBufferBlock.View = Graphics::MainCamera::Instance()->getViewMatrix();
    uniformBufferBlock.Projection = Graphics::MainCamera::Instance()->getProjectionMatrix();
	uniformBufferBlock.ViewProjection = Math::mat4::multiply(Graphics::MainCamera::Instance()->getViewMatrix(), Graphics::MainCamera::Instance()->getProjectionMatrix());

	uniformBufferBlock.InvView = Math::mat4::inverse(uniformBufferBlock.View);
	uniformBufferBlock.InvProjection = Math::mat4::inverse(uniformBufferBlock.Projection);
	uniformBufferBlock.InvViewProjection = Math::mat4::inverse(uniformBufferBlock.ViewProjection);

	uniformBufferBlock.CameraPos = Graphics::MainCamera::Instance()->GetPosition();

	uniformBufferBlock.ScreenSize = this->renderResolution;
	uniformBufferBlock.TimeAndRandom[0] = (GLfloat)glfwGetTime();

	//TODO: Add some interesting random function here!
	uniformBufferBlock.TimeAndRandom[1] = 0.0f; 

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
	std::weak_ptr<FramePass> framePass = FrameServer::Instance()->GetDepthPass();
    auto depthPass = framePass.lock();

    //Run depth pass
	depthPass->Execute();
	
	framePass = FrameServer::Instance()->GetLightCullingPass();
	auto lightCullingPass = framePass.lock();

	lightCullingPass->Execute();

	//Bind the final color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FrameServer::Instance()->finalColorFrameBufferObject);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#ifdef _LIGHT_DEBUG
	glUseProgram(this->lightDebugShader->GetProgram());

	glUniform1i(glGetUniformLocation(this->lightDebugShader->GetProgram(), "totalLightCount"), LightServer::Instance()->pointLights.Size());

	// Render Lights only within tiles
	for (Material* material : FrameServer::Instance()->FlatGeometryLit->materials)
	{
		for (ModelInstance* modelInstance : material->getModelInstances())
		{
			//Bind mesh
			modelInstance->GetMesh()->Bind();
			for (GraphicsProperty* graphicsProperty : modelInstance->GetGraphicsProperties())
			{
				material->GetShader(str)->setModelMatrix(graphicsProperty->getModelMatrix());
				modelInstance->GetMesh()->Draw();
			}
			modelInstance->GetMesh()->Unbind();
		}
	}
#else

	framePass = FrameServer::Instance()->FlatGeometryLit;
	auto flatGeometryLitPass = framePass.lock();

	//Run draw pass
	flatGeometryLitPass->Execute();

	//-------------------
	// Render Debug Shapes!
	Debug::DebugRenderer::Instance()->DrawCommands();


	glViewport(0, 0, windowResolution.x, windowResolution.y);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK); // Set the back buffer as the draw buffer

	//Copy final colorbuffer to screen if specified
	if (drawToScreen)
	{
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, FrameServer::Instance()->finalColorFrameBufferObject);
		glBlitFramebuffer(0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->windowResolution.x, this->windowResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		//This is only for OGL 4.5 and it might cause issues with older cards...
		//glBlitNamedFramebuffer(FrameServer::Instance()->finalColorFrameBufferObject, 0, 0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->windowResolution.x, this->windowResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

#endif

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
}




}