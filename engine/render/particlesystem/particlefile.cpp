#include "config.h"
#include "particlefile.h"
#include "render/particlesystem/particlesystem.h"
#include "toolkit/particleeditor/code/emittersUI.h"

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
		pEmitter->SetAttribute("name", this->emitters[i]->GetEmitterName().AsCharPtr());
		pSystem->InsertEndChild(pEmitter);

		pElement = xmlDoc.NewElement("Texture");
		pElement->SetAttribute("path", this->emitters[i]->GetTexturePath().AsCharPtr());
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
		pElement->SetAttribute("startSize", set.size[0]);
		pElement->SetAttribute("endSize", set.size[1]);
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
	XMLError eResult = xmlDoc.SaveFile(saveposition.AsCharPtr());
	this->emitters.Clear();
	XMLCheckResult(eResult);

	return true;
}

bool ParticleFile::SaveParticle(Util::String name,std::unordered_map<int, std::shared_ptr<ParticleEditor::EmittersUI>> ems)
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
	for (size_t i = 0; i < ems.size(); i++)
	{
		if (ems[i]->GetDummy())
			continue;
		ParticleUISettings set = ems[i]->settings;
		pEmitter = xmlDoc.NewElement("ParticleEmitter");
		pEmitter->SetAttribute("name", ems[i]->ev.name.AsCharPtr());
		pSystem->InsertEndChild(pEmitter);

		pElement = xmlDoc.NewElement("Texture");
		pElement->SetAttribute("path", ems[i]->settings.texName.AsCharPtr());
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
		pElement->SetAttribute("startSize", set.size[0]);
		pElement->SetAttribute("endSize", set.size[1]);
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

	Util::String saveposition = "resources/particles/" + name;
	XMLError eResult = xmlDoc.SaveFile(saveposition.AsCharPtr());
	this->emitters.Clear();
	XMLCheckResult(eResult);

	return true;
}

Util::Array<FileSettings> ParticleFile::LoadParticle(Util::String path)
{

	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(path.AsCharPtr());

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
	Util::String vecText;
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
		vecText = pValues->Attribute("acc");
		arr = vecText.Tokenize(" ");
		fileSet.set.acc = Math::vec4(arr[0].AsFloat(), arr[1].AsFloat(), arr[2].AsFloat(), 1.0);
		vecText = pValues->Attribute("acc2");
		arr = vecText.Tokenize(" ");
		fileSet.set.acc2 = Math::vec4(arr[0].AsFloat(), arr[1].AsFloat(), arr[2].AsFloat(), 1.0);
		fileSet.set.accRand = pValues->BoolAttribute("random");
		
		//Lifetime
		pValues = pElement->FirstChildElement("Lifetime");
		fileSet.set.acc[3] = pValues->FloatAttribute("life");
		fileSet.set.acc2[3] = pValues->FloatAttribute("life2");
		fileSet.set.lifeTimeRand = pValues->BoolAttribute("random");

		//Color
		pValues = pElement->FirstChildElement("Color");
		vecText = pValues->Attribute("color");
		arr = vecText.Tokenize(" ");
		fileSet.set.color = Math::vec4(arr[0].AsFloat(), arr[1].AsFloat(), arr[2].AsFloat(), arr[3].AsFloat());
		vecText = pValues->Attribute("color2");
		arr = vecText.Tokenize(" ");
		fileSet.set.color2 = Math::vec4(arr[0].AsFloat(), arr[1].AsFloat(), arr[2].AsFloat(), arr[3].AsFloat());
		fileSet.set.colorRand = pValues->BoolAttribute("random");

		//Size
		pValues = pElement->FirstChildElement("Size");
		fileSet.set.size[0] = pValues->FloatAttribute("startSize");
		fileSet.set.size[1] = pValues->FloatAttribute("endSize");

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

void ParticleFile::AppendEmitter(Ptr<Game::ParticleEmitter> emitter)
{
	if (std::find(this->emitters.begin(), this->emitters.end(), emitter) == this->emitters.end())
		this->emitters.Append(emitter);
}
}