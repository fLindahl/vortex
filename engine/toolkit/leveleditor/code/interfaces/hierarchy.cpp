#include "hierarchy.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "application/basegamefeature/managers/entitymanager.h"
#include "../toolhandler.h"
#include "../selecttool.h"

namespace Interface
{
	__ImplementClass(Interface::Hierarchy, 'Hier', Interface::InterfaceBase)

	Hierarchy::Hierarchy()
	{

	}

	Hierarchy::~Hierarchy()
	{

	}

	void Hierarchy::Run()
	{
		if (ImGui::Button("New Entity"))
		{
			Ptr<Game::Entity> newEntity = Game::Entity::Create();
			newEntity->Activate();
			BaseGameFeature::EntityManager::Instance()->RegisterEntity(newEntity);
		}
		ImGui::Separator();

		auto selectTool = Tools::ToolHandler::Instance()->SelectTool();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		{
			auto& list = BaseGameFeature::EntityManager::Instance()->GetEntityList();
			for (std::unordered_map<int, Ptr<Game::Entity>>::iterator it = list.begin(); it != list.end(); it++)
			{
				ImGui::PushID(it->first);
				if (ImGui::Selectable(it->second->GetClassName().AsCharPtr(), (selectTool->GetSelectedEntity()->GetID() == it->first)))
				{
					selectTool->SetSelectedEntity(it->second);
				}
				ImGui::PopID();
			}
			ImGui::EndChild();
		}
		
	}
}


