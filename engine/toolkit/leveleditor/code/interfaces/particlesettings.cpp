#include "particlesettings.h"
#include "imgui.h"
#include "imgui_dock.h"

namespace Interface
{
	__ImplementClass(Interface::ParticleSettings, 'PtSt', Interface::InterfaceBase)

	ParticleSettings::ParticleSettings()
	{

	}

	ParticleSettings::~ParticleSettings()
	{

	}

	void ParticleSettings::Run()
	{
		//this->particleCount = 0;
		//if (ImGui::Button("Save"))
		//{
		//	Particles::ParticleFile::Instance()->SaveParticle("testParticle");
		//}
		//if (ImGui::IsItemHovered())
		//{
		//	ImGui::BeginTooltip();
		//	ImGui::Text("Saves the current particles.\nSaves all the currently appended emitters\nthen removes them from the list");
		//	ImGui::EndTooltip();
		//}
		//for (int i = 0; i < application->particleList.Size(); ++i)
		//{
		//	//ParticlesSettings(application->particleList[i]->GetEmitters());
		//	this->particleCount++;
		//}
	}
}


