#include "sceneview.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "../basetool.h"
#include "../toolhandler.h"
#include "render/server/renderdevice.h"
#include "../selecttool.h"


//------------
// PHYSICS
#include "foundation/math/line.h"
#include "application/basegamefeature/managers/envmanager.h"
#include "physics/physicsserver.h"
#include "physics/rigidbodyproperty.h"

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


			//------------------------
			// PHYSICS
			if (ImGui::GetIO().MouseDown[1])
			{
				ImVec2 dockPos = ImGui::GetWindowPos();

				ImGuiStyle& style = ImGui::GetStyle();
				dockPos.x += style.WindowPadding.x;
				dockPos.y += style.WindowPadding.y;

				ImVec2 dockSize = ImGui::GetWindowSize();
				ImVec2 mouse_pos_in_dock = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);

				Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();

				Math::line rayLine = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mouse_pos_in_dock.x, mouse_pos_in_dock.y, 5000.0f, dockSize.x, dockSize.y);

				Physics::PhysicsHit newHit;
				if (Physics::PhysicsServer::Instance()->Raycast(newHit, rayLine))
				{
					_printf("--- Hit object! ---");

					const Ptr<Property::Rigidbody>& rbe = newHit.object->FindProperty<Property::Rigidbody>();

					if (rbe.isvalid())
						rbe->GetRigidBody()->applyForceAtPoint(Math::vec4::normalize(rayLine.vec()), .1f, newHit.point);
				}
			}

		}	
	}
}


