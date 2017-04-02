#pragma once
#include "entity.h"
#include "foundation/util/string.h"
#include "modelentity.h"


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

	void LoadEmitters(Util::String path);

	void SetTransform(const Math::mat4& t);

	void UpdatePosition(const Math::mat4 &t);
	
	Util::Array<std::shared_ptr<Property::ParticleEmitter>>& GetEmitters(){ return this->emitters; }

protected:
	
	Util::Array<std::shared_ptr<Property::ParticleEmitter>> emitters;
	
};
}
