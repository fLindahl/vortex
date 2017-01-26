#include "config.h"
#include "particlesystem.h"
#include "render/server/resourceserver.h"
#include "application/properties/particleemitter.h"
#include <random>

namespace Particles
{


ParticleSystem::ParticleSystem() : particleBuffer(0)
{
	glGenBuffers(1, &particleBuffer);
	glGenBuffers(1, &particleStartBuffer);
	mesh = Render::ResourceServer::Instance()->LoadMesh("resources/meshes/quad_tris.obj");
}

ParticleSystem::~ParticleSystem()
{
}

EmitterBuffer ParticleSystem::GetEmitterBuffer(index_t bufferSize, Property::ParticleEmitter& owner)
{

	emitters.InsertSorted(&owner);
	//Create a "buffer" for an emitter
	EmitterBuffer buf;
	buf.startIndex = particleArray.Size();
	particleArray.Reserve(bufferSize);
	particleArray.Fill(buf.startIndex, bufferSize, owner.GetState());
	buf.endIndex = particleArray.Size();
	buf.arr = &particleArray;
	buf.startArr = &particleStartSettings;

	index_t numPart = particleArray.Size();

	for (int i = buf.startIndex; i < buf.endIndex; i++)
	{
		particleArray[i].accLife[3] = RandomFloatValue(0.5f, 2.5f);
		particleArray[i].accLife[1] = RandomFloatValue(-9.82f, -7.82f);
		particleArray[i].accLife[0] = RandomFloatValue(-0.5f, 1.0f);
		particleArray[i].pos[0] = RandomFloatValue(-9.5f, 8.0f);
		particleArray[i].pos[2] = RandomFloatValue(-2.5f, 1.5f);
	}

	particleStartSettings = particleArray;

	// Bind particle buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleStartBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleArray[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleStartSettings[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return buf;
}

EmitterBuffer ParticleSystem::GetEmitterBuffer(index_t bufferSize, EmitterBuffer& eBuff)
{

	for (int i = eBuff.startIndex; i < eBuff.endIndex; i++)
	{
		particleArray.RemoveIndex(i);
	}

	//Create a "buffer" for an emitter
	EmitterBuffer buf;
	buf.startIndex = particleArray.Size();
	particleArray.Reserve(bufferSize);
	buf.endIndex = particleArray.Size();
	buf.arr = &particleArray;
	buf.startArr = &particleStartSettings;

	

	index_t numPart = particleArray.Size();

	// Bind particle buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleArray[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return buf;
}

float ParticleSystem::RandomFloatValue(float a, float b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(a, b);
	return dis(gen);
}
}



