#include "config.h"
#include "shadowmap.h"
#include "render/server/frameserver.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/frame/flatgeometrylitpass.h"


namespace Render
{
	ShadowMap::ShadowMap()
	{
		this->frameBufferObject = 0;
		this->buffer = 0;
		this->shadowmap = 0;
		this->shadowmapFBO = 0;
	}

	ShadowMap::~ShadowMap()
	{
		//empty
	}

	void ShadowMap::Setup()
	{
		///GENERATE A SHADOWMAP TEXTURE
		glGenFramebuffers(1, &this->frameBufferObject);

		glGenTextures(1, &this->buffer);
		glBindTexture(GL_TEXTURE_2D, this->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			1024,
			1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		///PUT THE SHADOW MAP INTO A FRAMEBUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->buffer, 0);
		///DONT NEED COLOR BUFFER
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		///FRAMEBUFFER CHECK
		GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(e == GL_FRAMEBUFFER_COMPLETE, "Render ShadowMap Framebuffer Status Error!");
		///UNBIND BUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		///SETUP LIGHTSPACE MATRIX FOR THE SHADER
		/*Render::LightServer::SpotLight sLight;
		sLight.position = Math::vec4(-5.5f, 3.5f, -0.9f, 1.0f);
		sLight.color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		sLight.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
		sLight.length = 10.0f;
		sLight.angle = 15.0f;
		LightServer::Instance()->AddSpotLight(sLight);
		
		///calculate light MVP from a single spotlight
		LightServer::SpotLight spotlight;

		if (LightServer::Instance()->GetNumSpotLights() != 0)
			spotlight = LightServer::Instance()->GetSpotLightAtIndex(0);

		Math::mat4 lightV, lightM, lightP;
		// TODO: FIX THIS SHET, MATH IS HARD YOU KNO'
		Math::point posss = Math::vec4(3.0f, 0.0f, 0.0f, 1.0f);
		lightV = Math::mat4::lookatrh(spotlight.position, spotlight.coneDirection, Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Render::Resolution res = Render::RenderDevice::Instance()->GetRenderResolution();
		float aspect = (float)res.x / (float)res.y;
		lightP = Math::mat4::perspfovrh(-Math::Deg2Rad(spotlight.angle), aspect, 0.05f, 1000.0f);
		///DO NOT REMOVE THE UNIFORM FROM THE H-FILE
		shadUniformBuffer.LSM = Math::mat4::multiply(lightV, lightP);
		shadUniformBuffer.lProj = lightP;*/
		///GENERATE A SHADOW BUFFER
		//glGenBuffers(1, this->ubo);
		
		
		///JUST FOR THE TIME BEING
		//this->buffer = shadowmap;
		//this->frameBufferObject = shadowmapFBO;
		
		this->sendtothisshaderprogram = ShaderServer::Instance()->LoadShader("defaultLit")->GetProgram();
		

		FramePass::Setup();
	}


	void ShadowMap::Execute()
	{
		this->BindFrameBuffer();

		///calculate light MVP from a single spotlight
		LightServer::SpotLight spotlight;

		if (LightServer::Instance()->GetNumSpotLights() != 0)
		{
			spotlight = LightServer::Instance()->GetSpotLightAtIndex(0);

			Math::mat4 lightV, lightM, lightP;
			Math::vec4 lookat;
			// TODO: FIX THIS SHET, MATH IS HARD YOU KNO'
			Math::point posss = Math::vec4::zerovector();
			lookat = spotlight.position + Math::vec4::multiply(spotlight.coneDirection, spotlight.length);
			lightV = Math::mat4::lookatrh(spotlight.position, lookat, Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			Render::Resolution res = Render::RenderDevice::Instance()->GetRenderResolution();
			float aspect = (float)res.x / (float)res.y;
			lightP = Math::mat4::perspfovrh(-Math::Deg2Rad(spotlight.angle), aspect, 0.05f, spotlight.length);
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

		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetShadowMap()->GetBuffer());

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

