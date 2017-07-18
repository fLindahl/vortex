#include "config.h"
#include "interfacemanager.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "IO/console.h"

namespace Interface
{
	InterfaceManager::InterfaceManager()
	{

	}

	void InterfaceManager::RunAll()
	{
		//List all interfaces in windows menu
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::BeginMenu("Show"))
				{
					ImGui::MenuItem("Console", NULL, &IO::Console::Instance()->OpenRef());

					for (auto it : this->interfaces)
					{
						ImGui::MenuItem(it->GetName().AsCharPtr(), NULL, &it->Open());
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}



		//------------------------------------------------------
		//Run all interfaces
		for (auto it : this->interfaces)
		{
			if (ImGui::BeginDock(it->GetName().AsCharPtr(), &it->Open(), ImGuiWindowFlags_NoSavedSettings | it->GetAdditionalFlags()))
			{
				it->Run();
			}
			ImGui::EndDock();
		}
	}

	void InterfaceManager::RegisterInterface(const Util::String & className, const char * label)
	{
		Ptr<Core::RefCounted> ptr = Core::Factory::Instance()->Create(className);
		Ptr<InterfaceBase> intFace = ptr.downcast<InterfaceBase>();
		_assert2(intFace != nullptr, "Interface could not be found by provided class name!");

		intFace->SetName(label);
		this->interfaces.Append(intFace);
	}

	void InterfaceManager::RegisterInterface(const Util::FourCC fourcc, const char * label)
	{
		Ptr<Core::RefCounted> ptr = Core::Factory::Instance()->Create(fourcc);
		Ptr<InterfaceBase> intFace = ptr.downcast<InterfaceBase>();
		_assert2(intFace != nullptr, "Interface could not be found by provided FourCC");

		intFace->SetName(label);
		this->interfaces.Append(intFace);
	}

	void InterfaceManager::RegisterInterface(Ptr<InterfaceBase> base)
	{
		this->interfaces.Append(base);
	}

}