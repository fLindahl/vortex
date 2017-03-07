#include "config.h"
#include "particleemitter.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/resourceserver.h"
#include "render/particlesystem/particlefile.h"

namespace Property
{
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
	this->texture = Render::ResourceServer::Instance()->LoadTexture(texturepath);
	this->texPath = texturepath;
	this->state = Particles::ParticleState();
	this->buff = Particles::ParticleSystem::Instance()->GetEmitterBuffer(this->numOfParticles, *this);
	this->pSet.numParticles = numOfParticles;
	this->renderBuff.offset = this->buff.startIndex;
	UpdateUniformBuffer();
}

void ParticleEmitter::CreateEmitter(Particles::FileSettings& set)
{
	this->numOfParticles = set.set.numParticles;
	this->texture = Render::ResourceServer::Instance()->LoadTexture(set.texPath);
	this->texPath = set.texPath;
	this->state = Particles::ParticleState();

	this->name = set.name;
	this->buff = Particles::ParticleSystem::Instance()->GetEmitterBuffer(this->numOfParticles, *this, set.set);
	this->pSet = set.set;

	this->renderBuff.offset = this->buff.startIndex;
	this->renderBuff.textureAnimation = Math::vec4(set.set.framesPerRow, set.set.numberOfFrames, 0.0f, int(set.set.spriteSheetTex));
	UpdateUniformBuffer();
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

void ParticleEmitter::UpdateTexture(const char* texturePath)
{
	this->texture = Render::ResourceServer::Instance()->LoadTexture(texturePath);
	this->texPath = texturePath;
}

	Math::mat4 ParticleEmitter::GetModelMatrix()
{
	return this->owner->GetTransform();
}

}

