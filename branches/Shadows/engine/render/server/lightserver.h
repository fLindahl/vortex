/*
 * TODO: The lightTiles should be send to the shader for easier changes, on how many lights each tile can contain.
 *          The Update method should only calculate the lights that are moving or the ones that have been moved
 */

#pragma once
#include "GL/glew.h"
#include "foundation/math/point.h"
#include "foundation/util/array.h"
#include "foundation/math/math.h"
#include "foundation/math/vector4.h"


namespace Render
{


	struct VisibleIndex 
	{
		int index;
	};

	class LightServer
	{
	public:
	/// 16 Byte Alignment sensitive
	struct PointLight
	{
		Math::point color;
		Math::point position;
		Math::vec4 radiusAndPadding;
	};

	struct SpotLight
	{
		SpotLight() {};

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
        void UpdateSpotLight(SpotLight& sLight);
		LightServer::SpotLight& GetSpotLightAtIndex(const int& index);

		GLuint GetWorkGroupsX() { return this->workGroupsX; }
		GLuint GetWorkGroupsY() { return this->workGroupsY; }

		GLuint GetPointLightBuffer() { return this->pointLightBuffer; }
		GLuint GetSpotLightBuffer() { return this->spotLightBuffer; }
		GLuint GetVisiblePointLightIndicesBuffer() { return this->visiblePointLightIndicesBuffer; }
		GLuint GetVisibleSpotLightIndicesBuffer() { return this->visibleSpotLightIndicesBuffer; }

        GLuint GetTileLights() { return this->tileLights; }

        void Update();
	private:
		friend class RenderDevice;

        void UpdateSpotLightBuffer();
		void UpdateWorkGroups();
		void UpdatePointLightBuffer();


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

        float oneOverThree;
	};
}
