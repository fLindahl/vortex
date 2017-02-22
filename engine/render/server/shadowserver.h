#pragma once
#include "GL/glew.h"
#include "foundation/math/point.h"
#include "foundation/util/array.h"
#include "foundation/math/math.h"
#include "foundation/math/vector4.h"

namespace Game
{
	class ModelEntitySpotLight;
}

namespace Render
{
	

	class ShadowServer
	{
	private:
		ShadowServer();
		
	public:
		static ShadowServer* Instance()
		{
			static ShadowServer instance;
			return &instance;
		}

		ShadowServer(const ShadowServer&) = delete;
		void operator=(const ShadowServer&) = delete;

		void AppendSpotLightEntity(Game::ModelEntitySpotLight* entity);
		void RemoveSpotLightEntity(Game::ModelEntitySpotLight* itr);

		Util::Array<Game::ModelEntitySpotLight*>& GetSpotLightEntities();
	private:
		Util::Array<Game::ModelEntitySpotLight*> spotLightEntities;
        GLuint spotLighEntetiesSize;
	};
}
