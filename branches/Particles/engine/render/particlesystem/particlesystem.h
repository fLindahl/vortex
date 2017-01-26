#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4.h"
#include "foundation/util/array.h"
#include "foundation/math/quaternion.h"
#include "foundation/util/fixedarray.h"
#include "render/resources/meshresource.h"

namespace Property{ class ParticleEmitter; }

namespace Particles
{

struct ParticleState
{
	Math::vec4 pos = Math::vec4(0.0f, 11.0f, 0.0f, 1.0f);
	Math::vec4 vel = Math::vec4(0.0f, 0.1f, 0.0f, 1.0f);
	Math::vec4 rot = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Math::vec4 accLife = Math::vec4(0.0f, 1.0f, 0.0f, 5.0f);


};

struct ParticleRenderingBuffer
{
	Math::vec4 color;
	Math::vec4 size;
	GLuint texture;
};

struct EmitterBuffer
{
	int startIndex;
	int endIndex;
	Util::Array<ParticleState>* arr;
	Util::Array<ParticleState>* startArr;

};

class ParticleSystem
{
public:
	
	static ParticleSystem* Instance()
	{
		static ParticleSystem instance;
		return &instance;
	}

	EmitterBuffer GetEmitterBuffer(index_t bufferSize, Property::ParticleEmitter& owner);
	EmitterBuffer GetEmitterBuffer(index_t bufferSize, EmitterBuffer& eBuff);


	Util::Array<ParticleState>& GetParticleArray(){ return this->particleArray; }
	GLuint GetParticleBuffer(){ return particleBuffer; }
	GLuint GetParticleStartBuffer(){ return particleStartBuffer; }
	Util::Array<Property::ParticleEmitter*>& GetParticleEmitters(){ return this->emitters; }
	std::shared_ptr<Render::MeshResource>& GetMesh(){ return this->mesh; }

	float RandomFloatValue(float a, float b);

private:
	ParticleSystem();
	~ParticleSystem();

	// Used for storage buffer objects to hold particle data
	GLuint particleBuffer;
	GLuint particleStartBuffer;

	std::shared_ptr<Render::MeshResource> mesh;

	Util::Array<ParticleState> particleArray;
	Util::Array<ParticleState> particleStartSettings;

	Util::Array<Property::ParticleEmitter*> emitters;

};
}
