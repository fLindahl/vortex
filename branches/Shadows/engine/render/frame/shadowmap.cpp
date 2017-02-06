#include "config.h"
#include "shadowmap.h"
#include "render/server/frameserver.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/server/lightserver.h"

namespace Render
{
	ShadowMap::ShadowMap()
	{
		this->frameBufferObject = 0;
		this->buffer = 0;
	}

	ShadowMap::~ShadowMap()
	{
		//empty
	}

	void ShadowMap::Setup()
	{
		//Set the depthBias matrix;
		Math::vec4 row1, row2, row3, row4;

		row1.set(0.5, 0.0, 0.0, 0.0);
		row2.set(0.0, 0.5, 0.0, 0.0);
		row3.set(0.0, 0.0, 0.5, 0.0);
		row4.set(0.5, 0.5, 0.5, 1.0);

		shadUniformBuffer.DepthBias.set(row1, row2, row3, row4);

		glGenBuffers(1, this->ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
		//all shadow buffers are from 9001 and forward
		glBindBufferBase(GL_UNIFORM_BUFFER, 9, this->ubo[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(shadowUniformBuffer), &shadUniformBuffer, GL_STATIC_DRAW);

		//setup the shadow map frame buffer
		//this->shadowmapframebuffer = 0;
		glGenFramebuffers(1, &this->frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
		
		//Depth texture, so we can sample it later in the shader
		glGenTextures(1, &this->buffer);
		glBindTexture(GL_TEXTURE_2D, this->buffer);
		//get render resolution from render device
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->buffer, 0);

		glDrawBuffer(GL_NONE); //don't need any color buffer because its just supposed to render shadows, ergo black and white

		//const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1, &drawbuffers[0]);

		//failsafe, check if buffer is alright otherwise stop
		GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(e == GL_FRAMEBUFFER_COMPLETE, "ShadowMap Framebuffer Status Error!");

		FramePass::Setup();
	}


	void ShadowMap::Execute()
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

	void ShadowMap::UpdateResolution()
	{
		const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

		glBindTexture(GL_TEXTURE_2D, this->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, newRes.x, newRes.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
}

