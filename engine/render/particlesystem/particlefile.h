#pragma once
#include "tinyxml2.h"
#include <string>
#include "application/properties/particleemitter.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }
#endif

using namespace tinyxml2;

/***********************
 * Save and load function for particle system.
 * Singleton
 ***********************/
namespace Particles
{	
class ParticleFile
{
public:
	
	static ParticleFile* Instance()
	{
		static ParticleFile instance;
		return &instance;
	}

	~ParticleFile();

	bool SaveParticle(Util::String name);
	bool LoadParticle(Util::String path);
	void AppendEmitter(Ptr<Property::ParticleEmitter> emitter);

private:
	ParticleFile();

	Util::Array<Ptr<Property::ParticleEmitter>> emitters;
};
}

