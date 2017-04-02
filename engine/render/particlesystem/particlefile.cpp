#include "config.h"
#include <iostream>
#include <fstream>

#include "particlefile.h"
#include "render/particlesystem/particlesystem.h"


namespace Particles
{

ParticleFile::ParticleFile()
{
}

ParticleFile::~ParticleFile()
{
}

bool ParticleFile::SaveParticle(Util::String name)
{
	XMLDocument xmlDoc;
	XMLNode* pRoot = xmlDoc.NewElement("Vortex");
	xmlDoc.InsertFirstChild(pRoot);
	XMLElement* pSystem = xmlDoc.NewElement("ParticleSystem");
	pRoot->InsertFirstChild(pSystem);

	XMLElement* pEmitter;
	XMLElement* pElement;
	XMLElement* pShapeElements;
	std::string s;
	for (size_t i = 0; i < this->emitters.Size(); i++)
	{
		ParticleUISettings set = this->emitters[i]->GetParticleUISettings();
		pEmitter = xmlDoc.NewElement("ParticleEmitter");
		pEmitter->SetAttribute("name", this->emitters[i]->GetEmitterName());
		pSystem->InsertEndChild(pEmitter);

		pElement = xmlDoc.NewElement("Texture");

		Util::Array<Util::String> absPath = Util::String::Tokenize(this->emitters[i]->GetTexturePath(), "/");
		bool followingVals = false;
		Util::String path;
		for (int i = 0; i < absPath.Size(); i++)
		{
			if (absPath[i] == "resources")
			{
				followingVals = true;
			}
			
			if (followingVals && (i != absPath.Size() - 1))
			{
				path += absPath[i] + "/";
			}
			else if (followingVals && (i == absPath.Size() - 1))
			{
				path += absPath[i];
			}
		}

		pElement->SetAttribute("path", path.c_str());
		pElement->SetAttribute("framesPerRow", set.framesPerRow);
		pElement->SetAttribute("numberOfFrames", set.numberOfFrames);
		pElement->SetAttribute("spritesheet", set.spriteSheetTex);
		pEmitter->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("Velocity");
		pElement->SetAttribute("vel", set.vel);
		pElement->SetAttribute("vel2", set.vel2);
		pElement->SetAttribute("random", set.velRand);
		pEmitter->InsertEndChild(pElement);
	
		pElement = xmlDoc.NewElement("Acceleration");
		s = std::to_string(set.acc.x()) + " " + std::to_string(set.acc.y()) + " " + std::to_string(set.acc.z());
		pElement->SetAttribute("acc", s.c_str());
		s = std::to_string(set.acc2.x()) + " " + std::to_string(set.acc2.y()) + " " + std::to_string(set.acc2.z());
		pElement->SetAttribute("acc2", s.c_str());
		pElement->SetAttribute("random", set.accRand);
		pEmitter->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("Lifetime");
		pElement->SetAttribute("life", set.acc.w());
		pElement->SetAttribute("life2", set.acc2.w());
		pElement->SetAttribute("random", set.lifeTimeRand);
		pEmitter->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("Color");
		s = std::to_string(set.color.x()) + " " + std::to_string(set.color.y()) + " " + std::to_string(set.color.z()) + " " + std::to_string(set.color.w());
		pElement->SetAttribute("color", s.c_str());
		s = std::to_string(set.color2.x()) + " " + std::to_string(set.color2.y()) + " " + std::to_string(set.color2.z()) + " " + std::to_string(set.color2.w());
		pElement->SetAttribute("color2", s.c_str());
		pElement->SetAttribute("random", set.colorRand);
		pEmitter->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("Size");
		pElement->SetAttribute("startSize", set.startSize);
		pElement->SetAttribute("endSize", set.endSize);
		pEmitter->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("ParticleAmount");
		pElement->SetAttribute("num", set.numParticles);
		pEmitter->InsertEndChild(pElement);

		pElement = xmlDoc.NewElement("EmitterShapes");
		pElement->SetAttribute("shape", set.shapes);
		pEmitter->InsertEndChild(pElement);

		pShapeElements = xmlDoc.NewElement("Radius");
		pShapeElements->SetAttribute("radius", set.radius);
		pElement->InsertEndChild(pShapeElements);
	}

	Util::String saveposition = "resources/particles/" + name + ".particle";
	XMLError eResult = xmlDoc.SaveFile(saveposition.c_str());
	this->emitters.Clear();
	XMLCheckResult(eResult);

	json saveValues;

	return true;
}

Util::Array<FileSettings> ParticleFile::LoadParticle(Util::String path)
{

	

	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(path.c_str());

	assert(eResult == XML_SUCCESS);

	XMLNode * pRoot = xmlDoc.FirstChild();

	if (pRoot == nullptr)
	{
		printf("Error: %i\n", XML_ERROR_FILE_READ_ERROR);
		assert(false);
	}
	
	XMLElement* pSystem = pRoot->FirstChildElement("ParticleSystem");
	if (pSystem == nullptr)
	{
		printf("Error: %i\n", XML_ERROR_PARSING_ELEMENT);
		assert(false);
	}
	XMLElement* pElement = pSystem->FirstChildElement("ParticleEmitter");
	if (pElement == nullptr)
	{
		printf("Error: %i\n", XML_ERROR_PARSING_ELEMENT);
		assert(false);
	}
	
	Util::Array<Util::String> arr;
	Util::Array<FileSettings> settings;
	while (pElement != nullptr)
	{
		FileSettings fileSet;
		//Particle Name
		fileSet.name = pElement->Attribute("name");

		//Texture
		XMLElement* pValues = pElement->FirstChildElement("Texture");
		fileSet.texPath = pValues->Attribute("path");
		fileSet.set.framesPerRow = pValues->IntAttribute("framesPerRow");
		fileSet.set.numberOfFrames = pValues->IntAttribute("numberOfFrames");
		fileSet.set.spriteSheetTex = pValues->BoolAttribute("spritesheet");

		//Velocity
		pValues = pElement->FirstChildElement("Velocity");
		fileSet.set.vel = pValues->FloatAttribute("vel");
		fileSet.set.vel2 = pValues->FloatAttribute("vel2");
		fileSet.set.velRand = pValues->BoolAttribute("random");

		//Acceleration
		pValues = pElement->FirstChildElement("Acceleration");
		arr = Util::String::Tokenize(pValues->Attribute("acc"), " ");
		fileSet.set.acc = Math::vec4(arr[0].ToFloat(), arr[1].ToFloat(),arr[2].ToFloat(), 1.0);
		arr = Util::String::Tokenize(pValues->Attribute("acc2"), " ");
		fileSet.set.acc2 = Math::vec4(arr[0].ToFloat(), arr[1].ToFloat(), arr[2].ToFloat(), 1.0);
		fileSet.set.accRand = pValues->BoolAttribute("random");
		
		//Lifetime
		pValues = pElement->FirstChildElement("Lifetime");
		fileSet.set.acc[3] = pValues->FloatAttribute("life");
		fileSet.set.acc2[3] = pValues->FloatAttribute("life2");
		fileSet.set.lifeTimeRand = pValues->BoolAttribute("random");

		//Color
		pValues = pElement->FirstChildElement("Color");
		arr = Util::String::Tokenize(pValues->Attribute("color"), " ");
		fileSet.set.color = Math::vec4(arr[0].ToFloat(), arr[1].ToFloat(), arr[2].ToFloat(), arr[3].ToFloat());
		arr = Util::String::Tokenize(pValues->Attribute("color2"), " ");
		fileSet.set.color2 = Math::vec4(arr[0].ToFloat(), arr[1].ToFloat(), arr[2].ToFloat(), arr[3].ToFloat());
		fileSet.set.colorRand = pValues->BoolAttribute("random");

		//Size
		pValues = pElement->FirstChildElement("Size");
		fileSet.set.startSize = pValues->FloatAttribute("startSize");
		fileSet.set.endSize = pValues->FloatAttribute("endSize");

		//ParticleAmount
		pValues = pElement->FirstChildElement("ParticleAmount");
		fileSet.set.numParticles = pValues->IntAttribute("num");

		//EmitterShapes
		pValues = pElement->FirstChildElement("EmitterShapes");
		fileSet.set.shapes = (EmitterShapes) pValues->IntAttribute("shape");

		//Radius
		XMLElement* pShapes = pValues->FirstChildElement("Radius");
		fileSet.set.radius = pShapes->FloatAttribute("radius");

		settings.Append(fileSet);
		pElement = pElement->NextSiblingElement();
	}

	return settings;

}

void ParticleFile::AppendEmitter(std::shared_ptr<Property::ParticleEmitter> emitter)
{
	if (std::find(this->emitters.begin(), this->emitters.end(), emitter) == this->emitters.end())
		this->emitters.Append(emitter);
}


void ParticleFile::WriteToFile(Util::String filename, json j)
{
	std::ofstream fout("resources/particles/" + filename + ".particle");
	fout << j.dump();
}

json ParticleFile::LoadFromFile(Util::String path)
{
	std::ifstream ifs(path);
	json j = json::parse(ifs);
	return j;
}

}