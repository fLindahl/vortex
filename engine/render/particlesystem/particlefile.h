#pragma once
#include <string>

#include "tinyxml2.h"
#include "json/src/json.hpp"
#include "application/properties/particleemitter.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }
#endif

using namespace tinyxml2;
using json = nlohmann::json;

/***********************
 * Save and load function for particle system.
 * Singleton
 ***********************/
namespace Particles
{	
struct FileSettings
{
	ParticleUISettings set;
	Util::String name;
	Util::String texPath;
};

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
	Util::Array<FileSettings> LoadParticle(Util::String path);
	void AppendEmitter(std::shared_ptr<Property::ParticleEmitter> emitter);

private:
	ParticleFile();

	void WriteToFile(Util::String filename, json j);
	json LoadFromFile(Util::String path);

	Util::Array<std::shared_ptr<Property::ParticleEmitter>> emitters;
};
}

