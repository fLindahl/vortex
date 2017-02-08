/**--------------------
	LightServer | Singleton

	Handles all the lighting, including cubemap/environmentmap/lightprobe etc.
*/
#pragma once
#include "GL/glew.h"
#include "foundation/math/point.h"
#include "foundation/util/array.h"

namespace Render
{
	class CubeMapNode;

	struct PointLight
	{
		Math::point color;
		Math::point position;
		Math::vec4 radiusAndPadding;
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

		GLuint GetWorkGroupsX() { return this->workGroupsX; }
		GLuint GetWorkGroupsY() { return this->workGroupsY; }

		GLuint GetLightBuffer() { return this->lightBuffer; }
		GLuint GetVisibleLightIndicesBuffer() { return this->visibleLightIndicesBuffer; }

		void AddCubeMap(std::shared_ptr<CubeMapNode> node);
		void RemoveCubeMap(std::shared_ptr<CubeMapNode> node);

		std::shared_ptr<CubeMapNode> GetClosestCubemapToPoint(const Math::point& point);
		void RegenerateCubemaps();


	private:
		friend class RenderDevice;

		void UpdateWorkGroups();
		void UpdateLightBuffer();

		// Used for storage buffer objects to hold light data and visible light indicies data
		GLuint lightBuffer;
		GLuint visibleLightIndicesBuffer;

		// X and Y work group dimension variables for compute shader
		GLuint workGroupsX;
		GLuint workGroupsY;

		/// Contains all the pointlights in the game
		Util::Array<PointLight> pointLights;

		/// Contains all active cubemaps in the scene
		Util::Array<std::shared_ptr<CubeMapNode>> cubemapNodes;

	};
}
