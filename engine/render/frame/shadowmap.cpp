#include "config.h"
#include "shadowmap.h"
#include "render/server/frameserver.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/server/shadowserver.h"


namespace Render
{
	ShadowMap::ShadowMap()
	{
		this->frameBufferObject = 0;
		this->buffer = 0;
		this->shadowmap = 0;
		this->shadowmapFBO = 0;
		this->shadowWidth = 1024;
		this->shadowHeight = 1024;
		this->shadowAspect = this->shadowWidth / this->shadowHeight;
		this->shadowNearPlane = 0.05f;
	}

	ShadowMap::~ShadowMap()
	{
		//empty
	}

	void ShadowMap::Setup()
	{
		///GENERATE A SHADOWMAP TEXTURE
		glGenFramebuffers(1, &this->frameBufferObject);

        glGenTextures(1, &this->shadowmap);
        glBindTexture(GL_TEXTURE_2D, this->shadowmap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->shadowWidth, this->shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat bordercolor[] = { 1.0,1.0,1.0,1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

		///PUT THE SHADOW MAP INTO A FRAMEBUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->shadowmap, 0);
		///DONT NEED COLOR BUFFER
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		///FRAMEBUFFER CHECK
		GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(e == GL_FRAMEBUFFER_COMPLETE, "Render ShadowMap Framebuffer Status Error!");
		///UNBIND BUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->sendtothisshaderprogram = ShaderServer::Instance()->LoadShader("defaultLit")->GetProgram();

		FramePass::Setup();
	}


	void ShadowMap::Execute()
	{
		/// Set the view port of the shadow camera light
		glViewport(0.0f, 0.0f, this->shadowWidth, this->shadowHeight);

		this->BindFrameBuffer();

		///calculate light MVP from a single spotlight
		Game::ModelEntitySpotLight* spotlight;

		auto shadowEntities = ShadowServer::Instance()->GetSpotLightEntities();

		if (shadowEntities.Size() != 0)
		{
			spotlight = shadowEntities[0];

			Math::mat4 lightV, lightP;
			Math::vec4 lookat;
			/// TODO: FIX THIS SHET, MATH IS HARD YOU KNO'
			lookat = spotlight->GetTransform().get_position() + Math::vec4::multiply(spotlight->GetSpotLightDirection(), spotlight->GetSpotLightLength());
			lightV = Math::mat4::lookatrh(spotlight->GetTransform().get_position(), lookat, Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));

			//lightV = Math::mat4::transpose(spotlight->GetTransform());
			

			//lightP = Math::mat4::perspfovrh(-Math::Deg2Rad(spotlight.angle), this->shadowAspect, this->shadowNearPlane, spotlight.length);
            lightP = Math::mat4::perspfovrh(-Math::Deg2Rad(spotlight->GetSpotLightAngle() * 2.0f), this->shadowAspect, this->shadowNearPlane, spotlight->GetSpotLightLength() * 2.0f);
			///DO NOT REMOVE THE UNIFORM FROM THE H-FILE
			shadUniformBuffer.LSM = Math::mat4::multiply(lightV, lightP);
			shadUniformBuffer.lProj = lightP;
		}
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

			shader->EnableRenderState();
			shader->setUniMatrix4fv(shadUniformBuffer.LSM, "lightSpaceMatrix");
			shader->setUniMatrix4fv(shadUniformBuffer.lProj, "lightProj");

			for (auto surface : material->SurfaceList())
			{
				for (auto modelNode : surface->GetModelNodes())
				{
					//Bind mesh
					//TODO: We should probably check and make sure we don't bind these more than once
					///Bind mesh
					///TODO: We should probably check and make sure we don't bind these more than once
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

		glViewport(0.0f, 0.0f, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y);
		glUseProgram(sendtothisshaderprogram);

		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetShadowMap()->GetShadowMap());
		
		
		glUniform1i(glGetUniformLocation(sendtothisshaderprogram, "ShadowMap"), 9);
		const GLuint loc = (GLuint)glGetUniformLocation(this->sendtothisshaderprogram, "LSM");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &shadUniformBuffer.LSM.mat.m[0][0]);


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

