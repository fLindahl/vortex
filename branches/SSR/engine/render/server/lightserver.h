/*
 * TODO: The lightTiles should be send to the shader for easier changes, on how many lights each tile can contain.
 *          The Update method should only calculate the lights that are moving or the ones that have been moved
 *          The Directional Light is not yet added, only struct exists.
 */

#pragma once
#include "GL/glew.h"
#include "foundation/math/point.h"
#include "foundation/util/array.h"
#include "foundation/math/math.h"
#include "foundation/math/vector4.h"



namespace Render
{
	class CubeMapNode;

	struct VisibleIndex 
	{
		int index;
	};

	class LightServer
	{

	public:
	enum LightType
	{
		NaN = 1,
		Point = 2,
		Spot = 4,
		Directional = 8
	};

	/// 16 Byte Alignment sensitive
	struct PointLight
	{
		LightType lightType = LightType::Point;

		Math::point color;
		Math::point position;
		Math::vec4 radiusAndPadding;
	};
	/// Is not added to the shader yet
	struct DirectionalLight
	{
		LightType lightType = LightType::Directional;

		Math::point color;
		Math::point position;
	};

	struct SpotLight
	{
		LightType lightType = LightType::Spot;

		Math::point color;
		Math::point position;
		Math::vec4 coneDirection;
		Math::vec4 midPoint;
		float length;
		/// The bottom cirlces radius of the cone
		//float radius;
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
		LightServer::PointLight& GetPointLightAtIndex(const int& index);

		void AddSpotLight(SpotLight& pLight);
		size_t GetNumSpotLights() { return this->spotLights.Size(); }
        void CalculateSpotlight(SpotLight& sLight);
		LightServer::SpotLight& GetSpotLightAtIndex(const int& index);

		GLuint GetWorkGroupsX() { return this->workGroupsX; }
		GLuint GetWorkGroupsY() { return this->workGroupsY; }

		GLuint GetPointLightBuffer() { return this->pointLightBuffer; }
		GLuint GetSpotLightBuffer() { return this->spotLightBuffer; }
		GLuint GetVisiblePointLightIndicesBuffer() { return this->visiblePointLightIndicesBuffer; }
		GLuint GetVisibleSpotLightIndicesBuffer() { return this->visibleSpotLightIndicesBuffer; }

        GLuint GetTileLights() { return this->tileLights; }

		void UpdateSpotLightBuffer();
		void UpdatePointLightBuffer();


        /// Debug and Easier Access ///
        void CreateSpotLight(Math::point color, Math::point position, Math::vec4 direction, float length, float angle);
        void CreatePointLight(Math::point color, Math::point position, float radius);

		void AddCubeMap(std::shared_ptr<CubeMapNode> node);
		void RemoveCubeMap(std::shared_ptr<CubeMapNode> node);

		Util::Array<std::shared_ptr<CubeMapNode>>& GetClosestCubemapToPoint(const Math::point& point);
		void RegenerateCubemaps();

	private:
		friend class RenderDevice;
		
		//Sets the blendfactors for each influencing cubemap
		void CalculateBlendMapFactors();

		void UpdateWorkGroups();

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

        /// Used to calculate the Middle Point of the Spotlight
        float oneOverThree;

		/// Contains all active cubemaps in the scene
		Util::Array<std::shared_ptr<CubeMapNode>> cubemapNodes;

		// Contains all currently influencing cubemaps 
		Util::Array<std::shared_ptr<CubeMapNode>> selectedInfluenceVolumes;
	};
}
