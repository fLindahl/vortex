#pragma once
#include "entity.h"


namespace Render
{
class GraphicsProperty;
class ModelInstance;
}

namespace Game
{
class ParticleEntity : public Game::Entity
{

	ParticleEntity();
	~ParticleEntity();

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();

	void SetTransform(const Math::mat4& t);

protected:
	
};
}
