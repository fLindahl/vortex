#include "config.h"
#include "VSMshadowmap.h"
#include "render/server/frameserver.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/server/shadowserver.h"


namespace Render
{
	VSMShadowMap::VSMShadowMap()
	{
		this->frameBufferObject = 0;
		this->buffer = 0;
		this->framebuffer = 0;
		this->depthtexture = 0;
		this->VSMtexture = 0;
		this->shadowWidth = 2048;
		this->shadowHeight = 2048;
		this->shadowAspect = this->shadowWidth / this->shadowHeight;
		this->shadowNearPlane = 0.05f;
		this->MSAA = 4.0f;
	}

	VSMShadowMap::~VSMShadowMap()
	{
		//empty
	}

	void VSMShadowMap::Setup()
	{
		///MULTI FBO
		/*glEnable(GL_MULTISAMPLE);
		glGenFramebuffers(1, &multiFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, multiFBO);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		
		///FIRST GENERATE A MULTI 2D TEXTURE
		//glGenTextures(1, &this->MSMshadowmap);
		//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->MSMshadowmap);
		///GENERATE A RENDERBUFFER FOR DEPTH ATTACHMENT
		///SET 4 SAMPLES (4xMSAA) SAME AS IN WINDOW.CC
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(e == GL_FRAMEBUFFER_COMPLETE, "MSM ShadowMap MultiFramebuffer Status Error!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		///BIND THE BLIT BUFFER
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		///GENERATE A TEXTURE AND BIND IT TO THE BLIT FBO
		glGenTextures(1, &MSMshadowmap);
		glBindTexture(GL_TEXTURE_2D, MSMshadowmap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowWidth, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, MSMshadowmap, 0);*/

		glGenFramebuffers(1, &this->frameBufferObject);
		///VSM EXPERIMENTING
		glGenTextures(1, &depthtexture);
		glBindTexture(GL_TEXTURE_2D, depthtexture);
		// Remove artifact on the edges of the shadowmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		///color texture to overwrite with moments
		glGenTextures(1, &VSMtexture);
		glBindTexture(GL_TEXTURE_2D, VSMtexture);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG16, shadowWidth, shadowHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Remove artifact on the edges
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, shadowWidth, shadowHeight, 0, GL_RG, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		// bind fbo
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
		// attach the textures
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, VSMtexture, 0);
		
		GLenum k = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(k == GL_FRAMEBUFFER_COMPLETE, "VSM ShadowMap Framebuffer Status Error!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		this->sendtothisshaderprogram = ShaderServer::Instance()->LoadShader("defaultLit")->GetProgram();
		FramePass::Setup();
	}


	void VSMShadowMap::Execute()
	{
		//this->frameBufferObject = multiFBO;
		//glCullFace(GL_FRONT);
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
			lightP = Math::mat4::perspfovrh(-Math::Deg2Rad(spotlight->GetSpotLightAngle() * 2.0f), this->shadowAspect, this->shadowNearPlane, spotlight->GetSpotLightLength()*2);
			///DO NOT REMOVE THE UNIFORM FROM THE H-FILE
			shadUniformBuffer.LSM = Math::mat4::multiply(lightV, lightP);
			shadUniformBuffer.lProj = lightP;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		///DRAW STUFF HERE
		GLuint currentProgram = 0;

		for (Material* material : this->materials)
		{
			auto shader = material->GetShader(this->name);

			if (shader->GetProgram() != currentProgram)
			{
				currentProgram = shader->GetProgram(); //36
				glUseProgram(currentProgram);
			}

			shader->EnableRenderState();
			shader->setUniMatrix4fv(shadUniformBuffer.LSM, "lightSpaceMatrix");
			shader->setUniMatrix4fv(shadUniformBuffer.lProj, "lightProj");
			///SET UNIFORMS HERE
			for (auto surface : material->SurfaceList())
			{
				for (auto modelNode : surface->GetModelNodes())
				{
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

		//after its been rendered, do some type of filtering to it(curretnyl being made in the phong shader)


		glUseProgram(sendtothisshaderprogram);

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, this->VSMtexture);


		glUniform1i(glGetUniformLocation(sendtothisshaderprogram, "VSMShadowMap"), 10);
		const GLuint loc = (GLuint)glGetUniformLocation(this->sendtothisshaderprogram, "LSM");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &shadUniformBuffer.LSM.mat.m[0][0]);

		glCullFace(GL_BACK);
		glViewport(0.0f, 0.0f, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y);
		FramePass::Execute();
	}

	void VSMShadowMap::UpdateResolution()
	{

	}
}

