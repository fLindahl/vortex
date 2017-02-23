#include "imgui_dock.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "foundation/math/math.h"
#include "application/basegamefeature/managers/envmanager.h"
#include "render/frame/pickingpass.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace LevelEditor
{

void Application::CameraMovement()
{

	Math::vec4 translation = Math::vec4::zerovector();

	const float speedIncrease = 0.02f;
	float speedMultiplier = 1.0f;

	camRotX += ImGui::GetIO().MouseDelta.y * 0.005f;
	camRotY += ImGui::GetIO().MouseDelta.x * 0.005f;

	if (camRotX > 1.57075f)
	{
		camRotX = 1.57075f;
	}
	else if (camRotX < -1.57075f)
	{
		camRotX = -1.57075f;
	}

	if (ImGui::GetIO().KeyShift)
	{
		speedMultiplier = 3.0f;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W)))
	{
		translation.z() += speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
	{
		translation.z() -= speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
	{
		translation.x() -= speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
	{
		translation.x() += speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_UpArrow)))
	{
		translation.y() += speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_DownArrow)))
	{
		translation.y() -= speedIncrease * speedMultiplier;
	}

	Math::mat4 xMat = Math::mat4::rotationx(camRotX);
	Math::mat4 yMat = Math::mat4::rotationy(camRotY);
	Math::mat4 rotation = Math::mat4::multiply(xMat, yMat);

	//Math::mat4 rotation = Math::mat4::rotationyawpitchroll(nvgDegToRad(keyhandler->mouseY) * 0.5f, nvgDegToRad(keyhandler->mouseX) * 0.5f, 0.0f);

	const Math::point& left = rotation.get_xaxis();
	const Math::point& up = rotation.get_yaxis();
	const Math::point& forward = rotation.get_zaxis();

	translation = Math::mat4::transform(translation, rotation);
	cameraPos += translation;

	Graphics::MainCamera::Instance()->SetPosition(cameraPos);
	Graphics::MainCamera::Instance()->LookAt(cameraPos + forward, up);
}

void Application::DoPicking()
{

	ImVec2 dockPos = ImGui::GetWindowPos();

	ImGuiStyle& style = ImGui::GetStyle();
	dockPos.x += style.WindowPadding.x;
	dockPos.y += style.WindowPadding.y;

	ImVec2 dockSize = ImGui::GetWindowSize();
	ImVec2 mouse_pos_in_dock = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);

	//-------------------
	// Physics 

	if (ImGui::GetIO().MouseDown[1])
	{
		Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();
		
		Math::line rayLine = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mouse_pos_in_dock.x, mouse_pos_in_dock.y, 5000.0f, dockSize.x, dockSize.y);

		Physics::PhysicsHit newHit;
		if (Physics::PhysicsServer::Instance()->Raycast(newHit, rayLine))
		{
			this->rayStart = rayLine.start();

			printf("--- Hit object! ---\n");

			hit = newHit;

			Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(hit.object);

			if (rbe != nullptr)
				rbe->GetRigidBody()->applyForceAtPoint(Math::vec4::normalize(rayLine.vec()), .1f, hit.point);
			
			this->rayEnd = hit.point;
		}
		else
		{
			rayStart = cameraPos;
			rayEnd = rayLine.end();
		}
				
	}
	


	//---------------------
	// Picking
	
	if (ImGui::GetIO().MouseDown[0])
	{		
		GLuint SelectedID = 0;

		int pixelx = (mouse_pos_in_dock.x / dockSize.x) * Render::RenderDevice::Instance()->GetRenderResolution().x;
		int pixely = (mouse_pos_in_dock.y / dockSize.y) * Render::RenderDevice::Instance()->GetRenderResolution().y;

		//glBindTexture(GL_TEXTURE_2D, texture);
		GLuint frame = Render::FrameServer::Instance()->GetPickingPass()->GetFrameBufferObject();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, frame);
		//glReadPixels(pixelx, pixely, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, (GLvoid*)&SelectedID);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		if (SelectedID != 0)
		{
			hit.object = BaseGameFeature::EntityManager::Instance()->GetEntityByID(SelectedID).get();
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
}

}