#include "config.h"
#include "rendershadowmap.h"
#include "render/server/frameserver.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/frame/shadowmap.h"


namespace Render
{
	RenderShadowMap::RenderShadowMap()
	{
		this->frameBufferObject = 0;
		this->buffer = 0;
	}

	RenderShadowMap::~RenderShadowMap()
	{
		//empty
	}

	void RenderShadowMap::Setup()
	{
		/*GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		glGenTextures(1, &this->buffer);
		glBindTexture(GL_TEXTURE_2D, this->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &this->frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->buffer, 0);


		//drawbuffers
		const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, &drawbuffers[0]);

		GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(e == GL_FRAMEBUFFER_COMPLETE, "Render ShadowMap Framebuffer Status Error!");
		//GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT

		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

		Render::LightServer::SpotLight sLight;
		sLight.position = Math::vec4(-5.5f, 3.5f, -0.9f, 1.0f);
		sLight.color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		sLight.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
		sLight.length = 10.0f;
		sLight.angle = 15.0f;
		LightServer::Instance()->AddSpotLight(sLight);

		//calculate light MVP from a single spotlight
		LightServer::SpotLight spotlight;

		if (LightServer::Instance()->GetNumSpotLights() != 0)
			spotlight = LightServer::Instance()->GetSpotLightAtIndex(0);

		Math::mat4 lightV, lightM, lightP;
		//Math::vec4 lookat = spotlight.position + (spotlight.coneDirection * spotlight.length);
		lightV = Math::mat4::lookatrh(spotlight.position, spotlight.coneDirection, Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//lightV = Math::mat4::inverse(lightV);
		lightM.set_position(spotlight.position);
		//lightM = Math::mat4::transpose(lightM);
		lightP = Math::mat4::perspfovrh(Math::Deg2Rad(spotlight.angle), (RenderDevice::Instance()->GetRenderResolution().x/ RenderDevice::Instance()->GetRenderResolution().y), 0.05f, spotlight.length);

		shadRUniformBuffer.lightMVP = Math::mat4::multiply(Math::mat4::multiply(lightP, lightM), lightV);
		//bind light MVP
		glGenBuffers(1, this->ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
		//all shadow buffers are from 9001 and forward
		glBindBufferBase(GL_UNIFORM_BUFFER, 8, this->ubo[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(shadowRUniformBuffer), &shadRUniformBuffer, GL_STATIC_DRAW);

		FramePass::Setup();
	}


	void RenderShadowMap::Execute()
	{
		GLuint currentProgram = 0;

		for (Material* material : this->materials)
		{
			auto shader = material->GetShader(this->name);

			if (shader->GetProgram() != currentProgram)
			{
				currentProgram = shader->GetProgram();
				glUseProgram(currentProgram);
			}

			/*shader->EnableRenderState();

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
					//TODO: We should probably check and make sure we don't bind these more than once
					modelNode->modelInstance->GetMesh()->Bind();

					for (GraphicsProperty* graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
					{
						shader->setModelMatrix(graphicsProperty->getModelMatrix());
						shader->setInvModelMatrix(graphicsProperty->getInvModelMatrix());
						modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
					}

					modelNode->modelInstance->GetMesh()->Unbind();
				}
			}*/
		}

		//bind shadowmap
		glActiveTexture(GL_TEXTURE9);
		glUniform1i(glGetUniformLocation(currentProgram, "ShadowMapTest"), 4);
		glBindTexture(GL_TEXTURE_2D, this->shadowMapBuffer);

		FramePass::Execute();
	}

	void RenderShadowMap::UpdateResolution()
	{
		const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

		glBindTexture(GL_TEXTURE_2D, this->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

