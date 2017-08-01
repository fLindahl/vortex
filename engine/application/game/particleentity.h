#pragma once
#include "entity.h"
#include "foundation/util/string.h"

namespace Game{ class ParticleEmitter; }

namespace Game
{
class ParticleEntity : public Game::Entity
{
__DeclareClass(ParticleEntity)
public:
	ParticleEntity();
	~ParticleEntity();

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();

	void LoadEmitters(Util::String path);

	void SetTransform(const Math::mat4& t);

	void UpdatePosition(const Math::mat4 &t);
	
	Util::Array<Ptr<Game::ParticleEmitter>>& GetEmitters(){ return this->emitters; }

protected:
	
	Util::Array<Ptr<Game::ParticleEmitter>> emitters;
	
};
}
