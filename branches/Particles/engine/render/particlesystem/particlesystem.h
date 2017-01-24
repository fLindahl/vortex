#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4.h"
#include "foundation/util/array.h"
#include "foundation/math/quaternion.h"
#include "foundation/util/fixedarray.h"

namespace Particles
{

struct ParticleState
{
	Math::vec4 pos;
	Math::vec4 vel;
	Math::quaternion rot;
	GLfloat lifetime;
	GLfloat acceleration[3];
};

struct ParticleRendering
{
	Math::vec4 color;
};

struct EmitterBuffer
{
	int startIndex;
	int endIndex;
	Util::Array<ParticleState>* arr;
};

class ParticleSystem
{
public:
	
	static ParticleSystem* Instance()
	{
		static ParticleSystem instance;
		return &instance;
	}

	EmitterBuffer GetEmitterBuffer(index_t bufferSize);

private:
	ParticleSystem();
	~ParticleSystem();

	// Used for storage buffer objects to hold particle data
	GLuint particleBuffer;

	Util::Array<ParticleState> particleArray;

};
}
