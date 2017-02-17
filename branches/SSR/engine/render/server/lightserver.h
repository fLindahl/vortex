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
		Math::point color;
		Math::point position;
		Math::vec4 radiusAndPadding;
	};
	/// Is not added to the shader yet
	struct DirectionalLight
	{
		Math::point color;
		Math::point position;
	};

	struct SpotLight
	{
		//centerAndRadius:
		//Contains the center of the cone bounding sphere and the radius of it's sphere.
		Math::vec4 centerAndRadius;

		//colorAndCenterOffset:
		//Color of the light and center offset
		//if angle is greater than 50 (tan(50) = 1) the distance from the cornerpoints to the center is greater than the distance to the spotlight position.
		//In this special case we need to apply a center offset for the position of the spotlight
		Math::vec4 colorAndCenterOffset;

		//params:
		//Contains direction, angle and the falloff radius of the cone
		//XY is direction (we can reconstruct Z because we know the direction is normalized and we know the sign of Z dir)
		//Z is cosine of cone angle and lightdir Z sign (Sign bit is used to store sign for the z component of the light direction)
		//W is falloff radius
		Math::vec4 params;

		//We can reconstruct position of spotlight by knowing that the top of the cone will be the 
		//bounding spheres radius away from it's center in the direction of the spotlight 
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
		void RemovePointLight(PointLight* light);
		size_t GetNumPointLights() { return this->pointLights.Size(); }
		LightServer::PointLight& GetPointLightAtIndex(const int& index);

		void AddSpotLight(SpotLight& pLight);
		void RemoveSpotLight(SpotLight* light);
		size_t GetNumSpotLights() { return this->spotLights.Size(); }
		SpotLight CalculateSpotlight(Math::point color, Math::point position, Math::vec4 direction, float length, float angle);
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
		LightServer::SpotLight& CreateSpotLight(Math::point color, Math::point position, Math::vec4 direction, float length, float angle);
		LightServer::PointLight& CreatePointLight(Math::point color, Math::point position, float radius);

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

		/// Contains all active cubemaps in the scene
		Util::Array<std::shared_ptr<CubeMapNode>> cubemapNodes;

		// Contains all currently influencing cubemaps 
		Util::Array<std::shared_ptr<CubeMapNode>> selectedInfluenceVolumes;
	};
}
