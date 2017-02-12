#include "config.h"
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
	XMLNode* pRoot = xmlDoc.NewElement("ParticleSystem");
	xmlDoc.InsertFirstChild(pRoot);
	
	XMLElement* pEmitter;
	XMLElement* pElement;
	XMLElement* pShapeElements;
	std::string s;
	for (size_t i = 0; i < this->emitters.Size(); i++)
	{
		ParticleUISettings set = this->emitters[i]->GetParticleUISettings();
		pEmitter = xmlDoc.NewElement("ParticleEmitter");
		pEmitter->SetAttribute("name", this->emitters[i]->GetEmitterName());
		pRoot->InsertEndChild(pEmitter);

		pElement = xmlDoc.NewElement("Velocity");
		pElement->SetAttribute("vel", set.vel);
		pElement->SetAttribute("vel2", set.vel2);
		pElement->SetAttribute("random", set.vecRand);
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
		s = std::to_string(set.color.x()) + " " + std::to_string(set.color.y()) + " " + std::to_string(set.color.z());
		pElement->SetAttribute("color", s.c_str());
		s = std::to_string(set.color2.x()) + " " + std::to_string(set.color2.y()) + " " + std::to_string(set.color2.z());
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

	return true;
}

bool ParticleFile::LoadParticle(Util::String path)
{
	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(path.c_str());
	XMLCheckResult(eResult);

	XMLNode * pRoot = xmlDoc.FirstChild();

	if (pRoot == nullptr)
	{
		printf("Error: %i\n", XML_ERROR_FILE_READ_ERROR);
		return false;
	}
	
	XMLElement* pElement = pRoot->FirstChildElement("ParticleEmitter");
	if (pElement == nullptr)
	{
		printf("Error: %i\n", XML_ERROR_PARSING_ELEMENT);
		return false;
	}
	
	while (pElement != nullptr)
	{
		XMLElement* pValues = pElement->FirstChildElement("Velocity");


		

		pElement = pRoot->NextSiblingElement();
	}

	

}

	void ParticleFile::AppendEmitter(std::shared_ptr<Property::ParticleEmitter> emitter)
{
	if (std::find(this->emitters.begin(), this->emitters.end(), emitter) == this->emitters.end())
		this->emitters.Append(emitter);
}
}