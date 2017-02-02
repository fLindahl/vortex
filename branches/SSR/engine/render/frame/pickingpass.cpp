#include "config.h"
#include "render/server/frameserver.h"
#include "pickingpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/server/shaderserver.h"
#include "render/server/lightserver.h"
#include "render/frame/depthpass.h"

namespace Render
{

PickingPass::PickingPass()
{

}

PickingPass::~PickingPass()
{

}

void PickingPass::Execute()
{
	this->BindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT);

	//Because we're using the old depthbuffer from the depth prepass, we need to allow equal depth values, or clear the depth buffer. clearing would mean redundant work however.
	glDepthFunc(GL_LEQUAL);

	glUseProgram(this->shader->GetProgram());
	
	for (Material* material : this->materials)
	{
		for (auto surface : material->SurfaceList())
		{
			for (auto modelNode : surface->GetModelNodes())
			{
				//Bind mesh
				modelNode->modelInstance->GetMesh()->Bind();

				for (GraphicsProperty* graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
				{
					glUniform1ui(shaderIDLocation, (GLuint)graphicsProperty->GetOwner()->getID());
					shader->setModelMatrix(graphicsProperty->getModelMatrix());
					modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
				}

				modelNode->modelInstance->GetMesh()->Unbind();
			}
		}
	}

	glDepthFunc(GL_LESS);

	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    FramePass::Execute();
}

void PickingPass::Setup()
{
	this->shader = ShaderServer::Instance()->LoadShader("picking");

	glUseProgram(this->shader->GetProgram());
	shaderIDLocation = glGetUniformLocation(this->shader->GetProgram(), "ID");
	glUseProgram(0);

    glGenFramebuffers(1, &this->frameBufferObject);    
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glGenTextures(1, &this->buffer);
	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
	
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->buffer, 0);    
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetBuffer(), 0);

	//drawbuffers
	const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawbuffers[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "PickingPass Framebuffer Status Error!");

    FramePass::Setup();
}

void PickingPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, newRes.x, newRes.y, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

}