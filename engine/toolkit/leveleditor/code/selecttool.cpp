#include "selecttool.h"
#include "imgui.h"
#include "render/server/renderdevice.h"
#include "render/server/frameserver.h"

namespace Tools
{
	SelectTool::SelectTool()
	{
		type = ToolType::SELECT;
		this->currentHandle = TransformHandle::NONE;
	}

	SelectTool::~SelectTool()
	{
	}

	void SelectTool::Pick()
	{
		ImVec2 dockPos = ImGui::GetWindowPos();

		ImGuiStyle& style = ImGui::GetStyle();
		dockPos.x += style.WindowPadding.x;
		dockPos.y += style.WindowPadding.y;

		ImVec2 dockSize = ImGui::GetWindowSize();
		ImVec2 mouse_pos_in_dock = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);

		if (ImGui::GetIO().MouseDown[0])
		{
			GLuint SelectedID = 0;

			int pixelx = (mouse_pos_in_dock.x / dockSize.x) * Render::RenderDevice::Instance()->GetRenderResolution().x;
			int pixely = (mouse_pos_in_dock.y / dockSize.y) * Render::RenderDevice::Instance()->GetRenderResolution().y;

			glBindTexture(GL_TEXTURE_2D, Render::FrameServer::Instance()->GetPickingPass()->GetBuffer());
			GLuint frame = Render::FrameServer::Instance()->GetPickingPass()->GetFrameBufferObject();

			glBindFramebuffer(GL_READ_FRAMEBUFFER, frame);
			glReadPixels(pixelx, pixely, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, (GLvoid*)&SelectedID);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

			if (SelectedID != 0)
			{
				this->SetSelectedEntity(BaseGameFeature::EntityManager::Instance()->GetEntityByID(SelectedID));
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void SelectTool::SetSelectedEntity(const Ptr<Game::Entity>& entity)
	{
		this->selectedEntity = entity;
	}

	const Ptr<Game::Entity>& SelectTool::GetSelectedEntity()
	{
		return this->selectedEntity;
	}
}