#include "sceneview.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "../basetool.h"
#include "../toolhandler.h"
#include "render/server/renderdevice.h"

namespace Interface
{
	__ImplementClass(Interface::SceneView, 'ScnV', Interface::InterfaceBase)

	SceneView::SceneView()
	{
		this->additionalFlags = ImGuiWindowFlags_(ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	}

	SceneView::~SceneView()
	{
	}

	void SceneView::Run()
	{
		ImVec2 dockSize = ImGui::GetWindowSize();
		ImGui::Image((void*)Render::RenderDevice::Instance()->GetFinalColorBuffer(), dockSize);

		Tools::ToolHandler::Instance()->CurrentTool()->UpdateHandlePositions();
		Tools::ToolHandler::Instance()->CurrentTool()->Render();

		if (ImGui::IsItemHovered())
		{
			/*
			if (application->hit.object != nullptr)
			{
				if (ImGui::GetIO().MouseClicked[0])
				{
					Tools::ToolHandler::Instance()->CurrentTool()->LeftDown();
					Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(application->hit.object->GetTransform());
					if (Tools::ToolHandler::Instance()->CurrentTool()->GetCurrentHandle() == Tools::TransformHandle::NONE)
					{
						application->DoPicking();
					}
				}
				if (ImGui::GetIO().MouseReleased[0])
				{
					Tools::ToolHandler::Instance()->CurrentTool()->LeftUp();
				}
				if (ImGui::GetIO().MouseDown[0])
				{
					Tools::ToolHandler::Instance()->CurrentTool()->Drag();

					delta = Tools::ToolHandler::Instance()->CurrentTool()->GetDeltaMatrix();
					Math::mat4 objTransform = this->application->hit.object->GetTransform();

					this->application->hit.object->SetTransform(Math::mat4::multiply(delta, objTransform));
					Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(application->hit.object->GetTransform());
				}
			}
			else
			{
				application->DoPicking();
				if (application->hit.object != nullptr)
				{
					Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(application->hit.object->GetTransform());
				}
			}
			*/
		}	
	}
}


