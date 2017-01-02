#include "imgui_dock.h"
#include "application.h"
#include "render/server/frameserver.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace LevelEditor
{

void Application::RenderDocks()
{
	ImGui::RootDock(ImVec2(0, 16), ImVec2(window->GetWidth(), window->GetHeight() - 16));

	ImGui::BeginDock("3D View", NULL, ImGuiWindowFlags_NoSavedSettings);
	{
		ImVec2 dockSize = ImGui::GetWindowSize();
		
		ImGui::Image((void*)Render::FrameServer::Instance()->GetFinalColorBuffer(), dockSize);
	}
	ImGui::EndDock();

	//ImGui::BeginDock("Top View", NULL, ImGuiWindowFlags_NoSavedSettings);
	//ImGui::EndDock();

	//ImGui::BeginDock("Right View", NULL, ImGuiWindowFlags_NoSavedSettings);
	//ImGui::EndDock();

	//ImGui::BeginDock("Front View", NULL, ImGuiWindowFlags_NoSavedSettings);
	//ImGui::EndDock();

	ImGui::BeginDock("Inspector", NULL, ImGuiWindowFlags_NoSavedSettings);
	{
		static bool enabled = true;
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::Checkbox("Checkbox", &enabled);
	}
	ImGui::EndDock();

	ImGui::BeginDock("Layers", NULL, ImGuiWindowFlags_NoSavedSettings);
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Text("MousePos: (%g, %g)", io.MousePos.x, io.MousePos.y);
		ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f)   { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i))          { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i))         { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("MouseWheel: %.1f", io.MouseWheel);

		ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { ImGui::SameLine(); ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]); }
		ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))             { ImGui::SameLine(); ImGui::Text("%d", i); }
		ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))            { ImGui::SameLine(); ImGui::Text("%d", i); }
	}
	ImGui::EndDock();

	ImGui::BeginDock("Content Browser", NULL, ImGuiWindowFlags_NoSavedSettings);
	if (ImGui::Button("New Entity", { 100, 40 }))
	{
		std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Math::point(0.0f, -0.5f, -1.5f), this->modelInstance1);
		commandManager.DoCommand(command);
	}
	ImGui::EndDock();
}

}