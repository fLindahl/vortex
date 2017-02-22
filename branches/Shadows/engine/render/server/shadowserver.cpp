#include "config.h"
#include "shadowserver.h"
#include "renderdevice.h"
#include "render/resources/cubemapnode.h"
#include "foundation/math/math.h"

namespace Render
{

	ShadowServer::ShadowServer(): spotLighEntetiesSize(0)
	{
		
	}

	void ShadowServer::AppendSpotLightEntity(Game::ModelEntitySpotLight* entity)
	{
		this->spotLightEntities.Append(entity);
        this->spotLighEntetiesSize++;
	}

	void ShadowServer::RemoveSpotLightEntity(Game::ModelEntitySpotLight* itr)
	{
		this->spotLightEntities.RemoveSwap(this->spotLightEntities.Find(itr));
	}

	Util::Array<Game::ModelEntitySpotLight*>& ShadowServer::GetSpotLightEntities()
	{
		return this->spotLightEntities;
	}
}