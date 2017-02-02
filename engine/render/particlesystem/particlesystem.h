#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4.h"
#include "foundation/util/array.h"
#include "render/resources/meshresource.h"


namespace Property{ class ParticleEmitter; }
namespace Render{ class ShaderObject; }

namespace Particles
{

struct ParticleState
{
	Math::vec4 pos = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Math::vec4 vel = Math::vec4(0.0f, 0.1f, 0.0f, 1.0f);
	Math::vec4 rot = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Math::vec4 accLife = Math::vec4(0.0f, 1.0f, 0.0f, 0.5f);
	Math::vec4 color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);

};

struct ParticleRenderingBuffer
{
	Math::vec4 startSize = Math::vec4(0.1f,0.1f,0.1f,1.0f);
	Math::vec4 endSize = Math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	uint offset = 0;
};


struct ParticleUISettings
{
	float vel = 1;
	Math::vec4 acc;
	Math::vec4 color;
	float vel2 = 3.0f;
	Math::vec4 acc2 = Math::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Math::vec4 color2 = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	int numParticles = 10000;

	float radius = 1.0f;

	bool vecRand = false;
	bool accRand = false;
	bool colorRand = false;
	bool lifeTimeRand = false;

	float startSize = 0.05f;
	float endSize = 0.05f;

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
	void GetEmitterBuffer(index_t bufferSize, std::shared_ptr<Property::ParticleEmitter> owner, EmitterBuffer& eBuff);

	void DrawParticleSystem();

	void UpdateParticlePosition(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleVelocity(std::shared_ptr<Property::ParticleEmitter> owner, float min, float max, float radius, bool random = false);
	void UpdateParticleRotation(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleAcceleration(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleLifetime(std::shared_ptr<Property::ParticleEmitter> owner, float min, float max = 0, bool random = false);
	void UpdateParticleColor(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleSize(std::shared_ptr<Property::ParticleEmitter> owner, float start, float end);

	Util::Array<ParticleState>& GetParticleArray(){ return this->particleArray; }
	GLuint GetParticleBuffer(){ return particleBuffer; }
	GLuint GetParticleStartBuffer(){ return particleStartBuffer; }
	Util::Array<Property::ParticleEmitter*>& GetParticleEmitters(){ return this->emitters; }
	std::shared_ptr<Render::MeshResource> GetMesh(){ return this->mesh; }
	std::shared_ptr<Render::ShaderObject> GetParticleShaderObject();

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

	std::shared_ptr<Render::ShaderObject> sh;

};
}
