#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4.h"
#include "foundation/util/array.h"
#include "render/resources/meshresource.h"
#include "core/refcounted.h"


namespace Game{ class ParticleEmitter; }
namespace Render{ class ShaderObject; }

namespace Particles
{

//Emitting shapes of the particle system
enum EmitterShapes
{
	CONE,
	SPHERE,
	HEMISPHERE,
	MESH //TODO: IMPLEMENT ME!
};

//Physics state and settings of the particles in the compute shader
struct ParticleState
{
	Math::vec4 pos = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Math::vec4 vel = Math::vec4(0.0f, 0.1f, 0.0f, 1.0f);
	Math::vec4 rot = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Math::vec4 accLife = Math::vec4(0.0f, 1.0f, 0.0f, 0.5f);
	Math::vec4 color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Math::vec4 size = Math::vec4(0.1f,0.1f,0.1f,1.0f);
	Math::vec4 startSize = Math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	Math::vec4 endSize = Math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
};

//Uniform buffer used by the vertex shader
struct ParticleRenderingBuffer
{
	//Used to offset to the right texture per emitter 
	uint offset = 0;
	//x = number of frames per row
	//y = total number of frames
	//w = if it is a spreedsheet or not 1.0 = spreedsheet 0.0 = no spreedsheet
	Math::vec4 textureAnimation = Math::vec4(0.0f);
};

//Particles settings used by the UI to set up the particle state
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
	bool sizeOverTime = false;
	bool spriteSheetTex = false;

	float size[2];

	int framesPerRow = 1;
	int numberOfFrames = 1;

	EmitterShapes shapes = CONE;

	std::string texName = "";

};

//Sent to the emitter by the particle system
struct EmitterBuffer
{
	size_t startIndex;
	size_t endIndex;
	Util::Array<ParticleState>* arr;
	Util::Array<ParticleState>* startArr;

};

//Singleton class that handles all the particle emitters
class ParticleSystem : Core::RefCounted
{
__DeclareClass(ParticleSystem)
public:
	
	static ParticleSystem* Instance()
	{
		static ParticleSystem instance;
		return &instance;
	}

	EmitterBuffer GetEmitterBuffer(index_t bufferSize, Game::ParticleEmitter& owner);
	EmitterBuffer GetEmitterBuffer(index_t bufferSize, Game::ParticleEmitter& owner, ParticleUISettings& settings);
	void GetEmitterBuffer(index_t bufferSize, Ptr<Game::ParticleEmitter> owner, EmitterBuffer& eBuff);

	void DrawParticleSystem();

	void UpdateParticlePosition(Ptr<Game::ParticleEmitter> owner, Math::vec4 min, float radius, EmitterShapes shape);
	void UpdateParticleVelocity(Ptr<Game::ParticleEmitter> owner, float min, float max, float radius, EmitterShapes shape, bool random = false);
	void UpdateParticleRotation(Ptr<Game::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleAcceleration(Ptr<Game::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleLifetime(Ptr<Game::ParticleEmitter> owner, float min, float max = 0, bool random = false);
	void UpdateParticleColor(Ptr<Game::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleSize(Ptr<Game::ParticleEmitter> owner, float start, float end);

	Util::Array<ParticleState>& GetParticleArray(){ return this->particleArray; }
	GLuint GetParticleBuffer() const { return particleBuffer; }
	GLuint GetParticleStartBuffer() const { return particleStartBuffer; }
	Util::Array<Game::ParticleEmitter*>& GetParticleEmitters(){ return this->emitters; }
	Ptr<Render::MeshResource> GetMesh() const { return this->mesh; }
	Ptr<Render::ShaderObject> GetParticleShaderObject();

private:
	ParticleSystem();
	~ParticleSystem();

	// Used for storage buffer objects to hold particle data
	GLuint particleBuffer;
	// Used for storage buffer objects to hold particle data used when a particle spawnes
	GLuint particleStartBuffer;

	Ptr<Render::MeshResource> mesh;

	//Array of all emitters particle settings
	Util::Array<ParticleState> particleArray;
	//Array of all emitters particle start settings
	Util::Array<ParticleState> particleStartSettings;

	//List of emitters
	Util::Array<Game::ParticleEmitter*> emitters;

	Ptr<Render::ShaderObject> sh;

};
}
