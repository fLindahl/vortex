#include "config.h"
#include "application/game/entity.h"
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
__ImplementClass(Render::PickingPass, 'PIKP', Render::DrawPass);

PickingPass::PickingPass()
{

}

PickingPass::~PickingPass()
{

}

void PickingPass::Execute()
{
	this->BindFrameBuffer();

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert2(e == GL_FRAMEBUFFER_COMPLETE, "PickingPass Framebuffer Status Error!");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint currentProgram = 0;

	for (Material* material : this->materials)
	{
		if (!material->SurfaceList().IsEmpty())
		{
			auto shader = material->GetShader(this->name);

			if (shader->GetProgram() != currentProgram)
			{
				currentProgram = shader->GetProgram();
				glUseProgram(currentProgram);
			}

			shader->EnableRenderState();

			for (auto surface : material->SurfaceList())
			{
				for (uint i = 0; i < surface->TextureList().Size(); i++)
				{
					surface->TextureList()[i]->BindTexture(i);
					//TODO: This is kinda bad
					if (i == 0)
						glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_ALBEDOMAP), i);
					else if (i == 1)
						glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_NORMALMAP), i);
					else if (i == 2)
						glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_SPECULARMAP), i);
					else if (i == 3)
						glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_ROUGHNESSMAP), i);
				}

				for (auto modelNode : surface->GetModelNodes())
				{
					//Bind mesh
					modelNode->modelInstance->GetMesh()->Bind();

					for (GraphicsProperty* graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
					{
						glUniform1ui(glGetUniformLocation(shader->GetProgram(), "ID"), (GLuint)graphicsProperty->GetOwner()->GetID());
						shader->setModelMatrix(graphicsProperty->getModelMatrix());
						modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
					}

					modelNode->modelInstance->GetMesh()->Unbind();
				}
			}
		}
	}

	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    FramePass::Execute();
}

void PickingPass::Setup()
{
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
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FrameServer::Instance()->GetDepthPass()->GetBuffer());

	//drawbuffers
	const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawbuffers[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert2(e == GL_FRAMEBUFFER_COMPLETE, "PickingPass Framebuffer Status Error!");

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