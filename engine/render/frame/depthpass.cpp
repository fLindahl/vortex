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
            for (ModelInstance* modelInstance : surface->getModelInstances())
            {
                //Bind mesh
                //TODO: We should probably check and make sure we don't bind these more than once
                modelInstance->GetMesh()->Bind();

                for (GraphicsProperty* graphicsProperty : modelInstance->GetGraphicsProperties())
                {
                    shader->setModelMatrix(graphicsProperty->getModelMatrix());
                    modelInstance->GetMesh()->Draw();
                }

                modelInstance->GetMesh()->Unbind();
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
    glGenTextures(1, &this->buffer);

    glBindTexture(GL_TEXTURE_2D, this->buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->buffer, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    FramePass::Setup();
}

}