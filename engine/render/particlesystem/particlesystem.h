#pragma once
#include "core/refcounted.h"
#include "GL/glew.h"
#include "foundation/math/vector4.h"
#include "foundation/util/array.h"
#include "render/resources/meshresource.h"


namespace Property{ class ParticleEmitter; }
namespace Render{ class ShaderObject; }

namespace Particles
{

enum EmitterShapes
{
	CONE,
	SPHERE,
	HEMISPHERE
};

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
	ParticleUISettings()
	{
		size[0] = 0.05f;
		size[1] = 0.05f;
	}

	float vel = 1.0f;
	Math::vec4 acc = Math::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Math::vec4 color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float vel2 = 3.0f;
	Math::vec4 acc2 = Math::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Math::vec4 color2 = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	int numParticles = 10000;

	float radius = 1.0f;

	bool velRand = false;
	bool accRand = false;
	bool colorRand = false;
	bool lifeTimeRand = false;
	bool spriteSheetTex = false;
	bool sizeOverTime = false;

	float size[2];

	int framesPerRow = 1;
	int numberOfFrames = 1;

	EmitterShapes shapes = CONE;

	std::string texName = "";

};

struct EmitterBuffer
{
	size_t startIndex;
	size_t endIndex;
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
	void GetEmitterBuffer(index_t bufferSize, Ptr<Property::ParticleEmitter> owner, EmitterBuffer& eBuff);

	void DrawParticleSystem();

	void UpdateParticlePosition(Ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleVelocity(Ptr<Property::ParticleEmitter> owner, float min, float max, float radius, EmitterShapes shape, bool random = false);
	void UpdateParticleRotation(Ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleAcceleration(Ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleLifetime(Ptr<Property::ParticleEmitter> owner, float min, float max = 0, bool random = false);
	void UpdateParticleColor(Ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleSize(Ptr<Property::ParticleEmitter> owner, float start, float end);

	Util::Array<ParticleState>& GetParticleArray(){ return this->particleArray; }
	GLuint GetParticleBuffer() const { return particleBuffer; }
	GLuint GetParticleStartBuffer() const { return particleStartBuffer; }
	Util::Array<Property::ParticleEmitter*>& GetParticleEmitters(){ return this->emitters; }
	Ptr<Render::MeshResource> GetMesh() const { return this->mesh; }
	Ptr<Render::ShaderObject> GetParticleShaderObject();

private:
	ParticleSystem();
	~ParticleSystem();

	// Used for storage buffer objects to hold particle data
	GLuint particleBuffer;
	GLuint particleStartBuffer;

	Ptr<Render::MeshResource> mesh;

	Util::Array<ParticleState> particleArray;
	Util::Array<ParticleState> particleStartSettings;

	Util::Array<Property::ParticleEmitter*> emitters;

	Ptr<Render::ShaderObject> sh;

};
}
