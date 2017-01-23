#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4.h"

namespace Particles
{

struct ParticleSettings
{
	Math::vec4 pos;
	Math::vec4 vel;
	Math::vec4 color;
	float speed;
	float lifetime;
	float gravity;
};

class ParticleSystem
{
public:
	
	static ParticleSystem* Instance()
	{
		static ParticleSystem instance;
		return &instance;
	}

private:
	ParticleSystem();
	~ParticleSystem();

};
}
