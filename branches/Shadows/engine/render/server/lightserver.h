//------------------------------------------------------------------------
// Copyright Fredrik Lindahl
// With contribution by Viktor Andersson
//------------------------------------------------------------------------


/*
 * TODO: The Update method should only calculate the lights that are moving or the ones that have been moved
 *       The Directional Light should not be an array. Should be a uniform in the phong.frag
 *       If you spam spawn lights you get segmentation fault, look in here -> /modelentityWithSpotLight/pointlightentity
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
        /// w in direction should be zero
		Math::point direction;
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

        /// Point Lights ///
		void AddPointLight(const PointLight& pLight);
		void RemovePointLight(PointLight* light);
		size_t GetNumPointLights() { return this->pointLights.Size(); }
		PointLight& GetPointLightAtIndex(const int& index);

        /// Spotlights ///
		void AddSpotLight(SpotLight& pLight);
		void RemoveSpotLight(SpotLight* light);
		size_t GetNumSpotLights() { return this->spotLights.Size(); }
		SpotLight CalculateSpotlight(Math::point color, Math::point position, Math::vec4 direction, float length, float angle);
		SpotLight& GetSpotLightAtIndex(const int& index);

        /// Directional Lights ///
        void AddDirectionalLight(Math::vec4 color, Math::vec4 direction);
        DirectionalLight& GetDirectionalLight() { return this->directionalLight; }

		GLuint GetWorkGroupsX() { return this->workGroupsX; }
		GLuint GetWorkGroupsY() { return this->workGroupsY; }

		GLuint GetPointLightBuffer() { return this->pointLightBuffer; }
		GLuint GetSpotLightBuffer() { return this->spotLightBuffer; }
        GLuint GetDirectionalLightBuffer() { return this->directionalLightBuffer; }

		GLuint GetVisiblePointLightIndicesBuffer() { return this->visiblePointLightIndicesBuffer; }
		GLuint GetVisibleSpotLightIndicesBuffer() { return this->visibleSpotLightIndicesBuffer; }

        GLuint GetTileLights() { return this->tileLights; }

		void UpdateSpotLightBuffer();
		void UpdatePointLightBuffer();
        void UpdateDirectionalLightBuffer();

        /// Debug and Easier Access ///
		LightServer::SpotLight& CreateSpotLight(Math::point color, Math::point position, Math::vec4 direction, float length, float angle);
		LightServer::PointLight& CreatePointLight(Math::point color, Math::point position, float radius);
        //void CreateDirectionalLight(Math::point color, Math::point direction);

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

		// X and Y work group dimension variables for compute shader
		GLuint workGroupsX;
		GLuint workGroupsY;

		/// Contains all the pointlights in the game
		Util::Array<PointLight> pointLights;
		Util::Array<SpotLight> spotLights;
        /// Should only be one in the scene
        DirectionalLight directionalLight;

        /// Determines how many light should be registred per tile
        GLuint tileLights = 512;

		/// Contains all active cubemaps in the scene
		Util::Array<std::shared_ptr<CubeMapNode>> cubemapNodes;

		// Contains all currently influencing cubemaps 
		Util::Array<std::shared_ptr<CubeMapNode>> selectedInfluenceVolumes;
	};
}
