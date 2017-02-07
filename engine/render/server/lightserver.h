/*
 * TODO: The lightTiles should be send to the shader for easier changes, on how many lights each tile can contain.
 */

#pragma once
#include "GL/glew.h"
#include "foundation/math/point.h"
#include "foundation/util/array.h"
#include "foundation/math/math.h"
#include "foundation/math/vector4.h"


namespace Render
{
	/// 16 Byte Alignment sensitive
	struct PointLight
	{
		Math::point color;
		Math::point position;
		Math::vec4 radiusAndPadding;
	};

	struct SpotLight
	{
		Math::point color;
		Math::point position;
		Math::vec4 coneDirection;
		Math::vec4 midPoint;
		float length;
        /// The bottom cirlces radius of the cone
		float radius;
		/// The Frustum Culling radius
		float fRadius;
		float angle;
	};

	struct VisibleIndex 
	{
		int index;
	};

	class LightServer
	{
	private:
		LightServer();
		
	public:
		static LightServer* Instance()
		{
			static LightServer instance;
			return &instance;
		}

		LightServer(const LightServer&) = delete;
		void operator=(const LightServer&) = delete;
		
		void AddPointLight(const PointLight& pLight);
		size_t GetNumPointLights() { return this->pointLights.Size(); }

		void AddSpotLight(SpotLight& pLight);
		size_t GetNumSpotLights() { return this->spotLights.Size(); }
		Util::Array<SpotLight> GetSpotLightArray() { return this->spotLights; }

		GLuint GetWorkGroupsX() { return this->workGroupsX; }
		GLuint GetWorkGroupsY() { return this->workGroupsY; }

		GLuint GetPointLightBuffer() { return this->pointLightBuffer; }
		GLuint GetSpotLightBuffer() { return this->spotLightBuffer; }
		GLuint GetVisiblePointLightIndicesBuffer() { return this->visiblePointLightIndicesBuffer; }
		GLuint GetVisibleSpotLightIndicesBuffer() { return this->visibleSpotLightIndicesBuffer; }

        GLuint GetTileLights() { return this->tileLights; }

	private:
		friend class RenderDevice;

		void UpdateWorkGroups();
		void UpdatePointLightBuffer();
		void UpdateSpotLightBuffer();

		// Used for storage buffer objects to hold light data and visible light indicies data
		GLuint pointLightBuffer;
		GLuint spotLightBuffer;
		GLuint visiblePointLightIndicesBuffer;
		GLuint visibleSpotLightIndicesBuffer;

		// X and Y work group dimension variables for compute shader
		GLuint workGroupsX;
		GLuint workGroupsY;

		/// Contains all the pointlights in the game
		Util::Array<PointLight> pointLights;
		Util::Array<SpotLight> spotLights;

        /// Determines how many light should be registred per tile
        GLuint tileLights = 512;
	};
}
