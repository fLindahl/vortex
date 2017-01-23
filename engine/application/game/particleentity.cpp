#include "config.h"
#include "particleentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{
ParticleEntity::ParticleEntity()
{
}

ParticleEntity::~ParticleEntity()
{
}

void ParticleEntity::Activate()
{
	Entity::Activate();
}

void ParticleEntity::Deactivate()
{
	Entity::Deactivate();
}

void ParticleEntity::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
}
}
