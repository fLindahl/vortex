#pragma once
#include "entity.h"


namespace Property{ class ParticleEmitter; }

namespace Game
{
class ParticleEntity : public Game::Entity
{
public:
	ParticleEntity();
	~ParticleEntity();

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();

	void SetTransform(const Math::mat4& t);
	
	Ptr<Property::ParticleEmitter>& GetEmitter(){ return this->emitter; }

protected:
	
	Ptr<Property::ParticleEmitter> emitter;
};
}
