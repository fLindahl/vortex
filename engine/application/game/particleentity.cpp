#include "config.h"
#include "particleentity.h"
#include "application/properties/particleemitter.h"
#include "render/particlesystem/particlefile.h"

namespace Game
{
__ImplementClass(Game::ParticleEntity, 'PAEN', Game::Entity)
ParticleEntity::ParticleEntity()
{
	this->emitters.Append(Game::ParticleEmitter::Create());
}

ParticleEntity::~ParticleEntity()
{
}

void ParticleEntity::Activate()
{
	for (Ptr<Game::ParticleEmitter> emitter : this->emitters)
	{
		this->AddProperty(emitter.upcast<Game::BaseProperty>());
		emitter->Activate();
	}
	
	Entity::Activate();
}

void ParticleEntity::Deactivate()
{
	for (auto emitter : this->emitters)
	{
		emitter->Deactivate();
	}
	Entity::Deactivate();
}

void ParticleEntity::LoadEmitters(Util::String path)
{
	Util::Array<Particles::FileSettings> set = Particles::ParticleFile::Instance()->LoadParticle(path);

	for (size_t i = 0; i < set.Size(); i++)
	{
		if (i != 0)
			this->emitters.Append(Game::ParticleEmitter::Create());

		Activate();
		this->emitters[i]->CreateEmitter(set[i]);
	}

}

void ParticleEntity::SetTransform(const Math::mat4 &t)
{
	Entity::SetTransform(t);
}

void ParticleEntity::UpdatePosition(const Math::mat4 &t)
{
	SetTransform(t);
	for (size_t i = 0; i < this->emitters.Size(); i++)
	{
		Particles::ParticleSystem::Instance()->UpdateParticlePosition(this->emitters[i], t.get_position(), this->emitters[i]->GetParticleUISettings().radius, this->emitters[i]->GetParticleUISettings().shapes);
	}
}
}
