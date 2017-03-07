#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4.h"
#include "foundation/util/array.h"
#include "render/resources/meshresource.h"


namespace Property{ class ParticleEmitter; }
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

	float startSize = 0.05f;
	float endSize = 0.05f;

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
class ParticleSystem
{
public:
	
	static ParticleSystem* Instance()
	{
		static ParticleSystem instance;
		return &instance;
	}

	EmitterBuffer GetEmitterBuffer(index_t bufferSize, Property::ParticleEmitter& owner);
	EmitterBuffer GetEmitterBuffer(index_t bufferSize, Property::ParticleEmitter& owner, ParticleUISettings& settings);
	void GetEmitterBuffer(index_t bufferSize, std::shared_ptr<Property::ParticleEmitter> owner, EmitterBuffer& eBuff);

	void DrawParticleSystem();

	void UpdateParticlePosition(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleVelocity(std::shared_ptr<Property::ParticleEmitter> owner, float min, float max, float radius, EmitterShapes shape, bool random = false);
	void UpdateParticleRotation(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random = false);
	void UpdateParticleAcceleration(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleLifetime(std::shared_ptr<Property::ParticleEmitter> owner, float min, float max = 0, bool random = false);
	void UpdateParticleColor(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max = Math::vec4(), bool random = false);
	void UpdateParticleSize(std::shared_ptr<Property::ParticleEmitter> owner, float start, float end);

	Util::Array<ParticleState>& GetParticleArray(){ return this->particleArray; }
	GLuint GetParticleBuffer() const { return particleBuffer; }
	GLuint GetParticleStartBuffer() const { return particleStartBuffer; }
	Util::Array<Property::ParticleEmitter*>& GetParticleEmitters(){ return this->emitters; }
	std::shared_ptr<Render::MeshResource> GetMesh() const { return this->mesh; }
	std::shared_ptr<Render::ShaderObject> GetParticleShaderObject();

private:
	ParticleSystem();
	~ParticleSystem();

	// Used for storage buffer objects to hold particle data
	GLuint particleBuffer;
	// Used for storage buffer objects to hold particle data used when a particle spawnes
	GLuint particleStartBuffer;

	std::shared_ptr<Render::MeshResource> mesh;

	//Array of all emitters particle settings
	Util::Array<ParticleState> particleArray;
	//Array of all emitters particle start settings
	Util::Array<ParticleState> particleStartSettings;

	//List of emitters
	Util::Array<Property::ParticleEmitter*> emitters;

	std::shared_ptr<Render::ShaderObject> sh;

};
}
