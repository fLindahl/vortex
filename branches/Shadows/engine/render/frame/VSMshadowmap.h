#pragma once
//------------------------------------------------------------------------
// Copyright Rasmus, Viktor.
//------------------------------------------------------------------------
#include "drawpass.h"
#include "foundation/math/matrix4.h"
#include "application/game/modelentityWithSpotlight.h"

namespace Render
{
	class VSMShadowMap : public DrawPass
	{
	public:
		VSMShadowMap();
		~VSMShadowMap();

		void Setup();
		void Execute();
		void UpdateResolution();

		///returns a moment shadow map, without filtering
		GLuint& GetVSMShadowMap() { return this->VSMtexture; }
	private:
		//Buffer objects and textures
		GLuint depthtexture;
		GLuint VSMtexture;
		GLuint maxaniso;
		GLuint framebuffer;

		float MSAA;

		GLfloat shadowWidth, shadowHeight;
		/// Aspect ratio of the shadow map texture
		GLfloat shadowAspect;
		GLfloat shadowNearPlane;

		struct  shadowUniformBuffer
		{
			Math::mat4 LSM;
			Math::mat4 lProj;
		}shadUniformBuffer;

		GLuint sendtothisshaderprogram;
		GLuint blurprogram;
		Util::Array<std::shared_ptr<Game::ModelEntitySpotLight>> spotlightEntities;
	};

}
