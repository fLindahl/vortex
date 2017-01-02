#include "imgui_dock.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "foundation/math/math.h"

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

		if (ImGui::IsItemHovered())
		{
			DoPicking();
		}
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

void Application::CameraMovement()
{

	Math::vec4 translation = Math::vec4::zerovector();

	const float speedIncrease = 0.02f;
	float speedMultiplier = 1.0f;

	camRotX += ImGui::GetIO().MouseDelta.y * 0.01f;
	camRotY += ImGui::GetIO().MouseDelta.x * 0.01f;

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
	ImVec2 dockSize = ImGui::GetWindowSize();
	ImVec2 mouse_pos_in_dock = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);
	if (ImGui::GetIO().MouseDown[0])
	{
		//mouse_pos_in_dock.x = Math::min(mouse_pos_in_dock.x, mouse_pos_in_dock.y);
				
		//TODO: This is unnecessary work
		Math::mat4 view = Graphics::MainCamera::Instance()->getViewMatrix();
		Math::mat4 invView = Math::mat4::inverse(view);
		Math::mat4 invProj = Math::mat4::inverse(Graphics::MainCamera::Instance()->getProjectionMatrix());
		Math::mat4 invViewProj = Math::mat4::multiply(invView, invProj);
		Math::mat4 viewProj = Math::mat4::multiply(view, Graphics::MainCamera::Instance()->getProjectionMatrix());

		double cursorPosX = 0.0f;
		double cursorPosY = 0.0f;

		printf("mouse x : %f\n", mouse_pos_in_dock.x / dockSize.x);

		// Transform to world coordinates
		cursorPosX = (((mouse_pos_in_dock.x / dockSize.x) - 0.5f) * 2.0f);
		cursorPosY = (((mouse_pos_in_dock.y / dockSize.y) - 0.5f) * 2.0f);
		Math::vec4 cursorTransform = Math::vec4(cursorPosX, cursorPosY, 1.0, 1.0f);

		printf("cursorpos screenspace : %f, %f, %f, %f\n", cursorTransform.x(), cursorTransform.y(), cursorTransform.z(), cursorTransform.w());

		cursorTransform = Math::mat4::transform(cursorTransform, invProj);
		Math::point ray = (cursorTransform * 0.01f);
		Math::vec4 rayWorldPos = Math::mat4::transform(ray, invView);

		printf("rayWorldPos: %f %f %f %f\n", rayWorldPos.x(), rayWorldPos.y(), rayWorldPos.z(), rayWorldPos.w());

		Math::vec4 rayDirection = rayWorldPos - invView.get_position();
		rayDirection = Math::vec4::normalize(rayDirection);

		this->rayStart = rayWorldPos;

		Physics::PhysicsHit newHit;
		if (Physics::PhysicsServer::Instance()->Raycast(newHit, rayWorldPos, rayDirection, 400.0f))
		{
			printf("--- Hit object! ---\n");

			//Start by removing outline from previous hit object
			if (hit.object != nullptr)
			{
				Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(hit.object);

				if (rbe != nullptr)
				{
					rbe->GetGraphicsProperty()->outline = false;
				}
			}

			hit = newHit;


			//Select new object!
			Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(hit.object);
			if (rbe != nullptr)
			{
				rbe->GetGraphicsProperty()->outline = true;
				rbe->GetRigidBody()->applyForceAtPoint(rayDirection, .1f, hit.point);
			}

			this->rayEnd = hit.point;
		}
		else
		{
			rayEnd = rayWorldPos + (rayDirection*10.0f);
		}
				
	}

}

}