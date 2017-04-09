#include "emittersUI.h"
#include "userinterface.h"
#include "imgui.h"
#include "render/server/resourceserver.h"

namespace ParticleEditor
{
EmittersUI::EmittersUI()
{
}

EmittersUI::EmittersUI(std::shared_ptr<UserInterface> ui, int id)
{
	this->ui = ui;
	this->id = id;

	name = "New Emitter";

	dupIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/copy.png");
	deleteIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/delete.png");

	visibleIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/visible.png");
	notVisibleIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/cancel.png");

	activeIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/active.png");
	inactiveIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/inactive.png");
}

EmittersUI::~EmittersUI()
{
}

void EmittersUI::DrawEmitter()
{
	std::string n = "##emitterframe" + std::to_string(id);
	ImGui::BeginChild(n.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() - 5, 40), true);
	{
		if (ev.active)
		{
			if (ImGui::ImageButton((void*)activeIcon->GetHandle(), ImVec2(15, 15)))
			{
				ev.active = false;
			}
		}
		else
		{
			if (ImGui::ImageButton((void*)inactiveIcon->GetHandle(), ImVec2(15, 15)))
			{
				ev.active = true;
				ui->UpdateActiveEmitter(this->id);
			}
		}
		ImGui::SameLine();
		n = "##name" + std::to_string(id);
		ImGui::InputText(n.c_str(), (char*)ev.name.c_str(), 512);

		ImGui::SameLine(ImGui::GetWindowWidth() - 115);
		if (ev.visible)
		{
			if (ImGui::ImageButton((void*)visibleIcon->GetHandle(), ImVec2(20, 20)))
			{
				ev.visible = false;
			}
		}
		else
		{
			if (ImGui::ImageButton((void*)notVisibleIcon->GetHandle(), ImVec2(20, 20)))
			{
				ev.visible = true;
			}
		}
		
		ImGui::SameLine(ImGui::GetWindowWidth() - 80);
		if (ImGui::ImageButton((void*)dupIcon->GetHandle(), ImVec2(20, 20)))
		{
			ui->DuplicateEmitter(*this);
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)deleteIcon->GetHandle(), ImVec2(20, 20)))
		{

		}
	}
	ImGui::EndChild();
}
}

