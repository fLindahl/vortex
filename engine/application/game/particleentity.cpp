#include "config.h"
#include "particleentity.h"
#include "application/properties/particleemitter.h"

namespace Game
{
ParticleEntity::ParticleEntity()
{
	this->emitter = Property::ParticleEmitter::Create();
}

ParticleEntity::~ParticleEntity()
{
}

void ParticleEntity::Activate()
{
	this->AddProperty(this->emitter.downcast<BaseProperty>());
	this->emitter->Activate();
	Entity::Activate();
}

void ParticleEntity::Deactivate()
{
	this->emitter->Deactivate();
	Entity::Deactivate();
}

void ParticleEntity::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
}
}
