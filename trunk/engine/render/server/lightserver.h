#pragma once
#include "foundation/math/point.h"
#include "foundation/util/array.h"


namespace Render
{
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
		
	private:
		friend class RenderDevice;
		/// Contains all the pointlights in the game
		Util::Array<PointLight> pointLights;


	};
}