#pragma once
//------------------------------------------------------------------------
// Copyright Rasmus, Viktor. Inte dunder.
// With contribution by Shaggy, se contribution comment in .cpp
//------------------------------------------------------------------------
#include "drawpass.h"
#include "foundation/math/matrix4.h"
#include "render/server/lightserver.h"
#include "application/game/modelentityWithSpotlight.h"

namespace Render
{
	class ShadowMap : public DrawPass
	{
	public:
		ShadowMap();
		~ShadowMap();

		void Setup();
		void Execute();

        void GenerateShadowMap();
		void UpdateResolution();


		GLuint& GetShadowMap() { return this->shadowmap; }
	private:
		//Uniform Buffer Object
		GLuint shadowmap;
		GLuint sendtothisshaderprogram;

		GLuint shadowAtlas;

		struct  shadowUniformBuffer
		{
			Math::mat4 LSM;
			Math::mat4 lProj;
		}shadUniformBuffer;

	    GLuint shadowMapProgram;

		GLfloat shadowWidth, shadowHeight;
		GLfloat shadowAtlasWidth, shadowAtlasHeight;
		/// Aspect ratio of the shadow map texture
		GLfloat shadowAspect;
		GLfloat shadowNearPlane;

		Util::Array<std::shared_ptr<Game::ModelEntitySpotLight>> spotlightEntities;
	};

}