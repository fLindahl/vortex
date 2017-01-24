#include "config.h"
#include "particlesystem.h"

#define NUMPARTICLES 1024 * 1024
#define WORK_GROUP_SIZE 128
#define NUMGROUPS (NUMPARTICLES / WORK_GROUP_SIZE)

Particles::ParticleSystem::ParticleSystem()
{
	glGenBuffers(1, &particleBuffer);
}

Particles::ParticleSystem::~ParticleSystem()
{
}

Particles::EmitterBuffer Particles::ParticleSystem::GetEmitterBuffer(index_t bufferSize)
{
	
	EmitterBuffer buf;
	buf.startIndex = particleArray.Size();
	particleArray.Reserve(bufferSize);
	buf.endIndex = particleArray.Size();
	buf.arr = &particleArray;

	index_t numPart = particleArray.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleArray[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return buf;
}


