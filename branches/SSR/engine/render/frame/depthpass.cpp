#include "config.h"
#include "render/server/frameserver.h"
#include "depthpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"

namespace Render
{

DepthPass::DepthPass()
{

}

DepthPass::~DepthPass()
{

}

void DepthPass::Execute()
{
	// Bind the depth map's frame buffer and draw the depth map to it
	this->BindFrameBuffer();
	glClear(GL_DEPTH_BUFFER_BIT);

    GLuint currentProgram = 0;

    for (Material* material : this->materials)
    {
        auto shader = material->GetShader(this->name);

        if (shader->GetProgram() != currentProgram)
        {
            currentProgram = shader->GetProgram();
            glUseProgram(currentProgram);
        }

        //TODO: Renderstates?
        for (auto surface : material->SurfaceList())
        {
            for (auto modelNode : surface->GetModelNodes())
            {
                //Bind mesh
                //TODO: We should probably check and make sure we don't bind these more than once
                modelNode->modelInstance->GetMesh()->Bind();

				for (GraphicsProperty* graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
                {
                    shader->setModelMatrix(graphicsProperty->getModelMatrix());
					modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
                }

				modelNode->modelInstance->GetMesh()->Unbind();
            }
        }
    }

	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    FramePass::Execute();
}

void DepthPass::Setup()
{
    glGenFramebuffers(1, &this->frameBufferObject);
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glGenTextures(1, &this->buffer);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glGenTextures(1, &this->linearDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->buffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->linearDepthBuffer, 0);
    
	const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawbuffers[0]);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "Depth Framebuffer Status Error!");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    FramePass::Setup();
}

void DepthPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, newRes.x, newRes.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, newRes.x, newRes.y, 0, GL_RED, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

}