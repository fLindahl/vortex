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
	VSMShadowMap::VSMShadowMap(): LSMBuffer(0), texOffsetBuffer(0), lightCoordinatesBuffer(0)
	{
		this->frameBufferObject = 0;
		this->buffer            = 0;
		this->framebuffer       = 0;
		this->depthtexture      = 0;
		this->shadowAtlas       = 0;

		this->shadowWidth  = 1024;
		this->shadowHeight = 1024;
		this->shadowAspect = this->shadowWidth / this->shadowHeight;

		this->shadowAtlasWidth  = 8192;
		this->shadowAtlasHeight = 8192;

		this->shadowMapTextureLocation = 9;

		this->maxColumns = (int)std::floor(this->shadowAtlasWidth / this->shadowWidth);
		this->maxRows    = (int)std::floor(this->shadowAtlasHeight / this->shadowHeight);
		this->maxShadows = this->maxColumns * this->maxRows;

		this->textureScale = ((float)this->shadowWidth / (float)this->shadowAtlasWidth);

		this->xOffset = 0;
		this->yOffset = 0;

		this->prevLightSize = 0;

		this->shadowNearPlane = 0.05f;
		this->MSAA            = 4.0f;

		glGenBuffers(1, &this->LSMBuffer);
		glGenBuffers(1, &this->texOffsetBuffer);
		glGenBuffers(1, &this->lightCoordinatesBuffer);
	}

	VSMShadowMap::~VSMShadowMap()
	{
		//empty
	}

	void VSMShadowMap::Setup()
	{
		glGenFramebuffers(1, &this->frameBufferObject);

		///VSM EXPERIMENTING
		/// Activating this texture will mess with the Texture Atlas
		glGenTextures(1, &this->depthtexture);
		glBindTexture(GL_TEXTURE_2D, this->depthtexture);

		// Remove artifact on the edges of the shadowmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, this->shadowAtlasWidth, this->shadowAtlasWidth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		/// Color texture to overwrite with moments
		glGenTextures(1, &this->shadowAtlas);
		glBindTexture(GL_TEXTURE_2D, this->shadowAtlas);

		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG16, shadowWidth, shadowHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/// Remove artifact on the edges
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, this->shadowAtlasWidth, this->shadowAtlasHeight, 0, GL_RG, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		/// Bind fbo
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
		/// Attach the textures
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthtexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->shadowAtlas, 0);
		
		GLenum k = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assert(k == GL_FRAMEBUFFER_COMPLETE, "VSM ShadowMap Framebuffer Status Error!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/// Stores the Render Program & the Shadow Map Generation program
		this->shadowMapRenderProgram = ShaderServer::Instance()->LoadShader("defaultLit")->GetProgram();
		this->shadowMapProgram = ShaderServer::Instance()->LoadShader("VSMshadowMap")->GetProgram();

		FramePass::Setup();
	}


	void VSMShadowMap::Execute()
	{
		glUseProgram(this->shadowMapProgram);
		auto shadowEntities = ShadowServer::Instance()->GetSpotLightEntities();

		if (shadowEntities.Size() >= 0)
		{
			if (ShadowServer::Instance()->GetNumShadowEntities() > this->maxShadows)
				printf("Maximum number of shadows has been reached! %i\n", this->maxShadows);

			//this->frameBufferObject = multiFBO;

			this->BindFrameBuffer();

			/// Clear the Depth Buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			///calculate light MVP from a single spotlight
			Game::ModelEntitySpotLight* spotlight;

			/// Reset the offset
			this->xOffset = 0;
			this->yOffset = 0;

			Math::mat4 lightV, lightP;
			Math::vec4 lookat;

			/// Add a new Shadow Maps MVP
			if (this->prevLightSize < shadowEntities.Size() && this->prevLightSize >= 0)
			{
				spotlight = shadowEntities[ShadowServer::Instance()->GetNumShadowEntities() - 1];

				this->CreateLightSpaceMatrix(spotlight, lookat, lightV, lightP);
				Math::mat4 ls = Math::mat4::multiply(lightV, lightP);
				Math::mat4 lp = lightP;
				this->AddNewLSM(ls, lp);

				/// Update the previousLightSize to be the current Size of Shadow Entities
				this->prevLightSize = (int)shadowEntities.Size();
			}

			/// Update the existing Shadwo Maps MVP
			for (unsigned int i = 0; i < shadowEntities.Size(); i++)
			{
				spotlight = shadowEntities[i];

				/// If the maximum colums for one row has been reached, add one to the yOffset to move down to Row(n)
				if (this->xOffset % this->maxColumns == 0 && this->xOffset > 0)
				{
					this->xOffset = 0;
					this->yOffset++;
				}

				/// Offset the different shadow maps in the Atlas
				glViewport(this->xOffset * this->shadowWidth, this->yOffset * this->shadowHeight, this->shadowWidth, this->shadowHeight);

				/// This just Re-Calculates the Light Space Matrix of the Light
				this->CreateLightSpaceMatrix(spotlight, lookat, lightV, lightP);
				this->LightSpaceMatrix[i] = Math::mat4::multiply(lightV, lightP);
				this->LightProjection[i] = lightP;

				glUniformMatrix4fv(glGetUniformLocation(this->shadowMapProgram, "lightSpaceMatrix"), 1, false, &this->LightSpaceMatrix[i].mat.m[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(this->shadowMapProgram, "lightProj"), 1, false, &this->LightProjection[i].mat.m[0][0]);

				/// TODO: At some point this needs to be a better solution
				for (Material *material : this->materials)
				{
					auto shader = material->GetShader(this->name);
					shader->EnableRenderState();

					for (auto surface : material->SurfaceList())
					{
						for (auto modelNode : surface->GetModelNodes())
						{
							modelNode->modelInstance->GetMesh()->Bind();
							for (GraphicsProperty *graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
							{
								shader->setModelMatrix(graphicsProperty->getModelMatrix());
								modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
							}
							modelNode->modelInstance->GetMesh()->Unbind();
						}
					}
				}
				/// Increase xOffset if it has a lower number than the amount of shadow casting entities in the level
				if (this->xOffset < shadowEntities.Size())
					this->xOffset++;
			}

			//after its been rendered, do some type of filtering to it(curretnyl being made in the phong shader)

			glViewport(0.0f, 0.0f, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y);
			glUseProgram(this->shadowMapRenderProgram);

			glUniform1i(glGetUniformLocation(this->shadowMapRenderProgram, "numShadows"), shadowEntities.Size());

			/// Activate the Atlas Texture in the Render Program
			glActiveTexture(GL_TEXTURE0 + this->shadowMapTextureLocation);
			glBindTexture(GL_TEXTURE_2D, this->shadowAtlas);

			glUniform1i(glGetUniformLocation(this->shadowMapRenderProgram, "VSMShadowMap"), this->shadowMapTextureLocation);

			/// Update the LightMatrixBuffer
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->LSMBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, shadowEntities.Size() * sizeof(Math::mat4), &this->LightSpaceMatrix[0], GL_STATIC_DRAW);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			/// Update the TextureOffsetBuffer
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->texOffsetBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, shadowEntities.Size() * sizeof(Math::vec4), &this->textureCoordinates[0], GL_STATIC_DRAW);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			/// Should be a buffer for the positions for the Lights in the scene #static.vert
			/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->lightCoordinatesBuffer);
            glBufferData(GL_SHADER_STORAGE_BUFFER, shadowEntities.Size() * sizeof(Math::vec4), 0, GL_STATIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/

			/// Bind Buffers
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 16, this->LSMBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 17, this->texOffsetBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 18, this->lightCoordinatesBuffer);

		}
		else
		{
			printf("You can not have a negative value of lights!\n");
		}

		FramePass::Execute();
	}

	void VSMShadowMap::UpdateResolution()
	{
		const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();
		glBindTexture(GL_TEXTURE_2D, this->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, newRes.x, newRes.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	void VSMShadowMap::CreateLightSpaceMatrix(Game::ModelEntitySpotLight* spotlight, Math::vec4& lookat, Math::mat4& view, Math::mat4& projection)
	{
		lookat = spotlight->GetTransform().get_position() + Math::vec4::multiply(spotlight->GetSpotLightDirection(), spotlight->GetSpotLightLength());
		view = Math::mat4::lookatrh(spotlight->GetTransform().get_position(), lookat, Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		projection = Math::mat4::perspfovrh(-Math::Deg2Rad(spotlight->GetSpotLightAngle() * 2.0f), this->shadowAspect, 0.05f, spotlight->GetSpotLightLength());
	}

	void VSMShadowMap::AddNewLSM(Math::mat4& LightSpaceMatrix, Math::mat4& LightProjection)
	{
		int xOffset = 0;
		int yOffset = 0;

		auto shadowEntities = ShadowServer::Instance()->GetSpotLightEntities();

		for (unsigned int i = 1; i < shadowEntities.Size(); i++)
		{
			if (xOffset % this->maxColumns == 0 && xOffset > 0)
			{
				xOffset = 0;
				yOffset++;
			}

			if (xOffset < shadowEntities.Size())
				xOffset++;
		}

		this->textureCoordinates.Append(Math::vec4(xOffset * this->textureScale, yOffset * this->textureScale, this->textureScale, 0.0f));

		this->LightSpaceMatrix.Append(LightSpaceMatrix);
		this->LightProjection.Append(LightProjection);
	}
}

