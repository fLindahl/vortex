#pragma once
#include "entity.h"

namespace Particle
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
