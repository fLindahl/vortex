#include "config.h"
#include "particleentity.h"
#include "application/properties/particleemitter.h"

namespace Game
{
ParticleEntity::ParticleEntity()
{
	this->emitter = std::make_shared<Property::ParticleEmitter>();
}

ParticleEntity::~ParticleEntity()
{
}

void ParticleEntity::Activate()
{
	this->AddProperty(this->emitter);
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
