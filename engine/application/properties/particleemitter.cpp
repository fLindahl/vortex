#include "config.h"
#include "particleemitter.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/resourceserver.h"

namespace Property
{
__ImplementClass(Property::ParticleEmitter, 'emit', Game::BaseProperty)

ParticleEmitter::ParticleEmitter()
{
	glGenBuffers(1, this->ubo);
	//BindRenderBuffer();
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
	this->renderBuff.offset = this->buff.startIndex;
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Particles::ParticleRenderingBuffer), &renderBuff, GL_STATIC_DRAW);
}

void ParticleEmitter::UpdateUniformBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Particles::ParticleRenderingBuffer), &renderBuff, GL_STATIC_DRAW);
}

void ParticleEmitter::BindUniformBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, this->ubo[0]);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(renderBuff), &renderBuff, GL_STATIC_DRAW);
}

Math::mat4 ParticleEmitter::GetModelMatrix()
{
	return this->owner->GetTransform();
}

}

