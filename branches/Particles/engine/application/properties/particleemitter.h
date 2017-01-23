#pragma once
//#include "application/game/baseproperty.h"

namespace Property
{
class ParticleEmitter
{
	ParticleEmitter();
	~ParticleEmitter();

	void FixedUpdate();
	void Update();

	void Activate();
	void Deactivate();
};
}
