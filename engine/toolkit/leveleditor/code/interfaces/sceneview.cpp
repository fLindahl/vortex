#include "sceneview.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "../basetool.h"
#include "../toolhandler.h"
#include "render/server/renderdevice.h"
#include "../selecttool.h"

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

		auto* selectTool = Tools::ToolHandler::Instance()->SelectTool();

		if (ImGui::IsItemHovered())
		{
			if (selectTool->GetSelectedEntity().isvalid())
			{
				if (ImGui::GetIO().MouseClicked[0])
				{
					Tools::ToolHandler::Instance()->CurrentTool()->LeftDown();
					Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(selectTool->GetSelectedEntity()->GetTransform());
					if (Tools::ToolHandler::Instance()->CurrentTool()->GetCurrentHandle() == Tools::TransformHandle::NONE)
					{
						selectTool->Pick();
					}
				}
				if (ImGui::GetIO().MouseReleased[0])
				{
					Tools::ToolHandler::Instance()->CurrentTool()->LeftUp();
				}
				if (ImGui::GetIO().MouseDown[0])
				{
					Tools::ToolHandler::Instance()->CurrentTool()->Drag();

					const auto& delta = Tools::ToolHandler::Instance()->CurrentTool()->GetDeltaMatrix();
					Math::mat4 objTransform = selectTool->GetSelectedEntity()->GetTransform();

					selectTool->GetSelectedEntity()->SetTransform(Math::mat4::multiply(delta, objTransform));
					Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(selectTool->GetSelectedEntity()->GetTransform());
				}
			}
			else
			{
				selectTool->Pick();
				if (selectTool->GetSelectedEntity().isvalid())
				{
					Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(selectTool->GetSelectedEntity()->GetTransform());
				}
			}
		}	
	}
}


