#pragma once
#include "application/game/baseproperty.h"
#include "render/particlesystem/particlesystem.h"
#include "foundation/util/string.h"


namespace Render
{
	class GraphicsProperty;
	class ModelInstance;
	class TextureResource;
}

namespace Particles
{
	struct FileSettings;
	class ParticleFile;
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
	void CreateEmitter(Particles::FileSettings& set);
	void UpdateUniformBuffer();
	void BindUniformBuffer();
	void UpdateTexture(const char* texturePath);
	
	Math::mat4 GetModelMatrix();

	GLuint& GetNumberOfParticles(){ return numOfParticles; }

	Particles::ParticleState& GetState(){ return state; }

	std::shared_ptr<Render::TextureResource>& GetEmitterTexture(){ return this->texture; }

	GLuint* GetUniformBuffer(){ return this->ubo; }
	Particles::EmitterBuffer& GetEmitterBuffer(){ return this->buff; }
	Particles::ParticleRenderingBuffer& GetRenderBuffer(){ return this->renderBuff; }
	Particles::ParticleUISettings& GetParticleUISettings(){ return this->pSet; }
	Util::String& GetEmitterName(){ return this->name; }
	Util::String& GetTexturePath(){ return this->texPath; }

private:
	//uniformBuffer
	GLuint ubo[1];

	GLuint numOfParticles;

	//The state of the particles
	Particles::ParticleState state;

	//The emitter buffer from the Particle System
	Particles::EmitterBuffer buff;

	//The uniform render buffer
	Particles::ParticleRenderingBuffer renderBuff;

	//Texture
	std::shared_ptr<Render::TextureResource> texture;

	//Saved Settings
	Particles::ParticleUISettings pSet;

	//Name of the emitter
	Util::String name = "Name";

	//Texture path
	Util::String texPath;

};
}
