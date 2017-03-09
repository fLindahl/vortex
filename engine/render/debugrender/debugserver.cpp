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
#include "render/frame/reflectionpass.h"
#include "render/frame/pickingpass.h"
#include "render/frame/shadowmap.h"
#include "render/frame/VSMshadowmap.h"

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

			if (ImGui::MenuItem("FlatGeometryLit Color")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetBuffer()); }
			if (ImGui::MenuItem("Depth")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetBuffer()); }
			if (ImGui::MenuItem("Linear Depth")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer()); }
			if (ImGui::MenuItem("Reflection Map")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetReflectionPass()->GetReflectionBuffer()); }
			//if (ImGui::MenuItem("Picking")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->pickingPass->GetBuffer()); }
			if (ImGui::MenuItem("Light Tiles")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(dynamic_cast<Render::LightCullingPass*>(Render::FrameServer::Instance()->GetLightCullingPass().get())->GetBuffer()); }
			if (ImGui::MenuItem("FlatGeometryLit Normals")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer()); }
			if (ImGui::MenuItem("FlatGeometryLit Specular")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer()); }
			if (ImGui::MenuItem("Shadow Map")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetShadowMap()->GetShadowMap());}
			if (ImGui::MenuItem("VSM Shadow Map")) { Render::RenderDevice::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetVSMShadowMap()->GetVSMShadowMap()); }
			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


}
