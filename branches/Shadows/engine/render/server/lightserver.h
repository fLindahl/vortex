/*
 * TODO: The Update method should only calculate the lights that are moving or the ones that have been moved
 *       The Directional Light should not be an array.
 *       If you spam spawn lights you get segmentation fault, look in here/modelentityWithSpotLight/pointlightentity
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
        /// w in direction should be zero
		Math::point direction;
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

        /// Point Lights ///
		void AddPointLight(const PointLight& pLight);
		size_t GetNumPointLights() { return this->pointLights.Size(); }
		PointLight& GetPointLightAtIndex(const int& index);

        /// Spotlights ///
		void AddSpotLight(SpotLight& pLight);
		size_t GetNumSpotLights() { return this->spotLights.Size(); }
        void CalculateSpotlight(SpotLight& sLight);
		SpotLight& GetSpotLightAtIndex(const int& index);

        /// Directional Lights ///
        void AddDirectionalLight(const DirectionalLight& dLight);
        size_t GetNumDirectionalLights() { return this->directionlLights.Size(); }
        DirectionalLight& GetDirectionalLightAtIndex(const int& index);

		GLuint GetWorkGroupsX() { return this->workGroupsX; }
		GLuint GetWorkGroupsY() { return this->workGroupsY; }

		GLuint GetPointLightBuffer() { return this->pointLightBuffer; }
		GLuint GetSpotLightBuffer() { return this->spotLightBuffer; }
        GLuint GetDirectionalLightBuffer() { return this->directionalLightBuffer; }
		GLuint GetVisiblePointLightIndicesBuffer() { return this->visiblePointLightIndicesBuffer; }
		GLuint GetVisibleSpotLightIndicesBuffer() { return this->visibleSpotLightIndicesBuffer; }
        GLuint GetVisibleDirectionalLightIndicesBuffer() { return this->visibleDirectionaltLightIndicesBuffer; }

        GLuint GetTileLights() { return this->tileLights; }

		void UpdateSpotLightBuffer();
		void UpdatePointLightBuffer();
        void UpdateDirectionalLightBuffer();

        /// Debug and Easier Access ///
        void CreateSpotLight(Math::point color, Math::point position, Math::vec4 direction, float length, float angle);
        void CreatePointLight(Math::point color, Math::point position, float radius);
        void CreateDirectionalLight(Math::point color, Math::point direction);

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
        GLuint directionalLightBuffer;
		GLuint visiblePointLightIndicesBuffer;
		GLuint visibleSpotLightIndicesBuffer;
        GLuint visibleDirectionaltLightIndicesBuffer;

		// X and Y work group dimension variables for compute shader
		GLuint workGroupsX;
		GLuint workGroupsY;

		/// Contains all the pointlights in the game
		Util::Array<PointLight> pointLights;
		Util::Array<SpotLight> spotLights;
        Util::Array<DirectionalLight> directionlLights;

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
