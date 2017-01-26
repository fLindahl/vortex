#include "config.h"
#include "particleemitter.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/resourceserver.h"

namespace Property
{
ParticleEmitter::ParticleEmitter()
{
	
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::FixedUpdate()
{
	BaseProperty::FixedUpdate();
}

void ParticleEmitter::Update()
{
	BaseProperty::Update();
}

void ParticleEmitter::Activate()
{
	BaseProperty::Activate();
}

void ParticleEmitter::Deactivate()
{
	BaseProperty::Deactivate();
}

void ParticleEmitter::CreateEmitter(GLuint numOfParticles, const char* texturepath)
{
	this->numOfParticles = numOfParticles;
	texture = Render::ResourceServer::Instance()->LoadTexture(texturepath);
	state = Particles::ParticleState();
	state.pos = owner->GetTransform().get_position();
	this->buff = Particles::ParticleSystem::Instance()->GetEmitterBuffer(this->numOfParticles, *this);
}

void ParticleEmitter::UpdateParticles()
{
	Util::Array<Particles::ParticleState> s;
	for (int i = buff.startIndex; i < buff.endIndex; i++)
	{
		s[i] = state;
		
	}
	buff.arr = &s;
	buff.startArr = &s;
}
}

