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
	// Get position from depthbuffer
	
	if (ImGui::GetIO().MouseDown[0])
	{
		Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();

		GLuint texture = Render::FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer();
		
		// Get depth buffer
		glBindTexture(GL_TEXTURE_2D, texture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, this->depthPixels);

		texture = Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer();

		glBindTexture(GL_TEXTURE_2D, texture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, this->normalPixels);

		int pixelx = (mouse_pos_in_dock.x / dockSize.x) * 1920;
		int pixely = (mouse_pos_in_dock.y / dockSize.y) * 1020;

		GLfloat pixelDepth = this->depthPixels[(int)(pixelx + (pixely * 1920) + 0.5f)];

		Math::vector normal;
		normal.x() = this->normalPixels[(uint)((pixelx * 3) + 0 + (pixely * (1920 * 3)))];
		normal.y() = this->normalPixels[(uint)((pixelx * 3) + 1 + (pixely * (1920 * 3)))];
		normal.z() = this->normalPixels[(uint)((pixelx * 3) + 2 + (pixely * (1920 * 3)))];

		normal = Math::mat4::transform(normal, Math::mat4::transpose(Graphics::MainCamera::Instance()->getView()));

		float far = 1000.0f;
		float near = 0.05f;

		float fn = far - near;

		//Calculate world pos
		Math::vec4 clipSpaceLocation;
		clipSpaceLocation.x() = ((float)pixelx / 1920.0f) * 2.0f - 1.0f;
		clipSpaceLocation.y() = ((float)pixely / 1020.0f) * 2.0f - 1.0f;
		clipSpaceLocation.z() = 1.0f;
		clipSpaceLocation.w() = 1.0f;
		Math::vec4 homogenousLocation = Math::mat4::transform(clipSpaceLocation, Graphics::MainCamera::Instance()->getInvProjection());
		Math::point viewSpacePosition = homogenousLocation;// *(1.0f / homogenousLocation.w());
		
		//Math::point worldPos = Math::mat4::transform(viewSpacePosition, invView);

		this->rayStart = Graphics::MainCamera::Instance()->GetPosition();//Math::mat4::transform(viewSpacePosition, Graphics::MainCamera::Instance()->getInvView());

		Math::point rayOrigin = (viewSpacePosition * pixelDepth);
		this->rayEnd = Math::mat4::transform(rayOrigin, invView);

		this->reflectStart = this->rayEnd;

		Math::mat4 textureScale = Math::mat4(	0.5f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.5f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f);

		float sx = 1920.0f / 2.0f;
		float sy = 1020.0f / 2.0f;

		float xOffset = Graphics::MainCamera::Instance()->getProjection().getrow2().x() * 1920.0f * -0.5f;
		float yOffset = Graphics::MainCamera::Instance()->getProjection().getrow2().y() * 1020.0f * 0.5f;

		Math::mat4  scrScale = Math::mat4(	sx, 0.0f, 0.0f, 0.0f,
											0.0f, -sy, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											sx - xOffset, sy + yOffset, 0.0f, 1.0f);

		//setPixelOffset(proj.column(2).xy() * viewportExtent * Vector2(-0.5f, 0.5f));

		Math::mat4 proj = (Math::mat4::multiply(Graphics::MainCamera::Instance()->getProjection(), scrScale));

		Math::vec4 position = rayOrigin;
		position = Math::mat4::transform(position, proj);

		printf("%f, %f, %f, %f\n", position.x(), position.y(), position.z(), position.w());
		position *= (1.0f / position.w());
		printf("%f, %f, %f, %f\n\n\n\n\n", position.x(), position.y(), position.z(), position.w());


		
		Math::vector viewDir = Math::vector::normalize(rayOrigin);

		//Reflect vector against normal
		Math::vector reflectionDir = Math::vec4::reflect(Math::vector::normalize(normal), viewDir); // normalize(-2 * dot(viewDir, viewSpaceNormal) * viewSpaceNormal + viewDir); 

		this->reflectEnd = Math::mat4::transform(rayOrigin + (reflectionDir * 10.0f), Graphics::MainCamera::Instance()->getInvView());

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
}

}