#pragma once
//------------------------------------------------------------------------
// Copyright Rasmus, Viktor. Inte dunder.
// With contribution by Shaggy, se contribution comment in .cpp
//
// NOTE: What ever changes you make in here should be applied to VSMshadowmap.h & VSMshadowmap.cpp
//------------------------------------------------------------------------

/*
 * TODO: Trash Handling, when you remove a light make sure you remove the shadow map & the texture offset coordinates
 * In the static.vert shader, is a hard-coded value of how many shadows you can have. 10 was the number when this was written
 * For VSM to work you need to have the same Shadow Map Atlas Resolution, the VSM widht & height of the atlas needs to be the same as in the shadowmap.h
 * The Scale of the texture should only be sent in once, Util::Array<Math::vec4> textureCoordinates; The scale is strored multiple times
 */

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

		void UpdateResolution();

        GLuint& GetShadowMapAtlasBuffer() { return this->shadowAtlas; }

        void CreateLightSpaceMatrix(Game::ModelEntitySpotLight* spotlight, Math::vec4& lookat, Math::mat4& view, Math::mat4& projection);
        void AddNewLSM(Math::mat4& LightSpaceMatrix, Math::mat4& LightProjection);
	private:
		//Uniform Buffer Object
		GLuint shadowMapRenderProgram;
        GLuint shadowMapProgram;

        GLuint LSMBuffer;
        GLuint texOffsetBuffer;
        GLuint lightCoordinatesBuffer;

        GLuint shadowMapTextureLocation;

		GLuint shadowAtlas;

		GLuint shadowWidth, shadowHeight;
		GLuint shadowAtlasWidth, shadowAtlasHeight;
        /// Max Rows, Columns and Shadow Maps in the Shadow Atlas.
        int maxRows, maxColumns, maxShadows;

		int prevLightSize;
        /// Scales the Shadow Atlas to correct dimensions in the shader.
		float textureScale;
        /// Offsets in the Shadow Atlas for the Shadow Maps
        int xOffset, yOffset;

		/// Aspect ratio of the shadow map texture
		GLuint shadowAspect;

        Util::Array<Math::mat4> LightProjection;
        Util::Array<Math::mat4> LightSpaceMatrix;
        /// These are the coordinates for the shader
        Util::Array<Math::vec4> textureCoordinates;

	};

}
