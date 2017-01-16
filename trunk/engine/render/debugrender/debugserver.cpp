#include "config.h"
#include "debugserver.h"
#include "imgui.h"
#include "render/server/renderdevice.h"
#include "render/server/frameserver.h"
#include "render/frame/depthpass.h"
#include "render/frame/drawpass.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/frame/lightcullingpass.h"
#include "render/frame/framepass.h"

namespace Debug
{
DebugServer::DebugServer()
{
	
}

void DebugServer::ImGuiDebugBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Show Framebuffer:");

			if (ImGui::MenuItem("Depth")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetBuffer()); }
			if (ImGui::MenuItem("Light Tiles")) {  }
			if (ImGui::MenuItem("FlatGeometryLit Color")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer()); }
			if (ImGui::MenuItem("FlatGeometryLit Normals")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer()); }

			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


}