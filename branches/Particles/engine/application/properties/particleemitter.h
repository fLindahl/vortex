#pragma once
#include "application/game/baseproperty.h"
#include "render/particlesystem/particlesystem.h"



namespace Render
{
	class GraphicsProperty;
	class ModelInstance;
	class TextureResource;
}

namespace Property
{
class ParticleEmitter : public Game::BaseProperty
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void FixedUpdate();
	void Update();

	void Activate();
	void Deactivate();

	void CreateEmitter(GLuint numOfParticles, const char* texturepath);

	GLuint GetNumberOfParticles(){ return numOfParticles; }

	Particles::ParticleState& GetState(){ return state; }

	std::shared_ptr<Render::TextureResource>& GetEmitterTexture(){ return this->texture; }

	void UpdateParticles();

private:

	GLuint numOfParticles;

	//The state of the particles
	Particles::ParticleState state;

	//The emitter buffer from the Particle System
	Particles::EmitterBuffer buff;

	std::shared_ptr<Render::TextureResource> texture;

};
}
