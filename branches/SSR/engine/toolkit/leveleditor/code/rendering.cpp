#include "imgui_dock.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "foundation/math/math.h"
#include "application/basegamefeature/managers/envmanager.h"
#include "render/frame/depthpass.h"
#include "render/frame/flatgeometrylitpass.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace LevelEditor
{

void Application::CameraMovement()
{

	Math::vec4 translation = Math::vec4::zerovector();

	const float speedIncrease = 0.02f;
	float speedMultiplier = 1.0f;

	camRotX += ImGui::GetIO().MouseDelta.y * -0.005f;
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
		translation.z() -= speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
	{
		translation.z() += speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
	{
		translation.x() += speedIncrease * speedMultiplier;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
	{
		translation.x() -= speedIncrease * speedMultiplier;
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
	/*
	ImVec2 dockPos = ImGui::GetWindowPos();

	ImGuiStyle& style = ImGui::GetStyle();
	dockPos.x += style.WindowPadding.x;
	dockPos.y += style.WindowPadding.y;

	ImVec2 dockSize = ImGui::GetWindowSize();
	ImVec2 mouse_pos_in_dock = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);
	if (ImGui::GetIO().MouseDown[0])
	{
		Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();
		
		Math::line rayLine = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mouse_pos_in_dock.x, mouse_pos_in_dock.y, 5000.0f, dockSize.x, dockSize.y);

		Physics::PhysicsHit newHit;
		if (Physics::PhysicsServer::Instance()->Raycast(newHit, rayLine))
		{
			this->rayStart = rayLine.start();

			printf("--- Hit object! ---\n");

			hit = newHit;
			
			this->rayEnd = hit.point;
		}
		else
		{
			rayStart = cameraPos;
			rayEnd = rayLine.end();
		}
				
	}
	*/


	//---------------------
	// Get position from depthbuffer


	ImVec2 dockPos = ImGui::GetWindowPos();

	ImGuiStyle& style = ImGui::GetStyle();
	dockPos.x += style.WindowPadding.x;
	dockPos.y += style.WindowPadding.y;

	ImVec2 dockSize = ImGui::GetWindowSize();
	ImVec2 mouse_pos_in_dock = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);
	if (ImGui::GetIO().MouseDown[0])
	{
		Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();

		Math::line rayLine = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mouse_pos_in_dock.x, mouse_pos_in_dock.y, 5000.0f, dockSize.x, dockSize.y);

		this->rayStart = rayLine.start();
		this->rayEnd = rayLine.end();
		
		GLuint texture = Render::FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer();
		
		// Get depth buffer
		glBindTexture(GL_TEXTURE_2D, texture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, this->depthPixels);

		texture = Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer();

		glBindTexture(GL_TEXTURE_2D, texture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, this->normalPixels);

		int pPos = (int)(mouse_pos_in_dock.x + (mouse_pos_in_dock.y * 1920) + 0.5f);

		int pixelx = (mouse_pos_in_dock.x / dockSize.x) * 1920;
		int pixely = (mouse_pos_in_dock.y / dockSize.y) * 1020;

		GLfloat pixelDepth = this->depthPixels[(int)(pixelx + (pixely * 1920) + 0.5f)];

		Math::vector normal;
		normal.x() = this->normalPixels[(uint)((pixelx * 3) + 0 + (pixely * (1920 * 3)))];
		normal.y() = this->normalPixels[(uint)((pixelx * 3) + 1 + (pixely * (1920 * 3)))];
		normal.z() = this->normalPixels[(uint)((pixelx * 3) + 2 + (pixely * (1920 * 3)))];

		//normal = Math::mat4::transform(normal, Graphics::MainCamera::Instance()->getView());

		//Calculate world pos
		Math::vec4 clipSpaceLocation;
		clipSpaceLocation.x() = (mouse_pos_in_dock.x / dockSize.x) * 2 - 1;
		clipSpaceLocation.y() = (mouse_pos_in_dock.y / dockSize.y) * 2 - 1;
		clipSpaceLocation.z() = -1.0;
		clipSpaceLocation.w() = 1.0f;
		Math::vec4 homogenousLocation = Math::mat4::transform(clipSpaceLocation, Graphics::MainCamera::Instance()->getInvProjection());
		Math::point viewSpacePosition = homogenousLocation * (1.0f / homogenousLocation.w());

		this->rayStart = Math::mat4::transform(viewSpacePosition, Graphics::MainCamera::Instance()->getInvView());

		float far = 1000.0f;
		float near = 0.05f;

		float fn = far * near;


		Math::point rayOrigin = viewSpacePosition * (pixelDepth * 35);
		this->rayEnd = Math::mat4::transform(rayOrigin, Graphics::MainCamera::Instance()->getInvView());

		this->reflectStart = this->rayEnd;

		
		Math::vector viewDir = Math::vector::normalize3(rayOrigin);

		//Reflect vector against normal
		Math::vector reflectionDir = Math::vec4::reflect(viewDir, Math::vector::normalize3(normal)); // normalize(-2 * dot(viewDir, viewSpaceNormal) * viewSpaceNormal + viewDir); 

		this->reflectEnd = this->rayEnd + Math::vector::normalize3(normal) * 1000;//Math::mat4::transform(rayOrigin + (reflectionDir * 100.0f), Graphics::MainCamera::Instance()->getInvView());

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

}