//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <fysik/physicsdevice.h>
#include <render/debugrender/debugrenderer.h>
#include "foundation/math/plane.h"
#include "imgui.h"
#include "render/server/frameserver.h"
#include "render/server/lightserver.h"

#include "application/basegamefeature/keyhandler.h"

using namespace Display;
using namespace Render;

#define CONSOLE_BUFFER_SIZE 8096

namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	cameraPos = Math::point::zerovector();
	camRotX = 0;
	camRotY = 0;
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	
	keyhandler = BaseGameFeature::KeyHandler::Instance();
	keyhandler->Init(this->window);
	
	this->rayStart = Math::vec4::zerovector();
	this->rayEnd = Math::vec4::zerovector();

	// Initiate everything we need
	// TODO: We should be able to cut down on a lot of this code by creating our own resource structures
	if (this->window->Open())
	{
		//Init RenderDevice
		RenderDevice::Instance()->Initialize();
		// Setup framepasses
		FrameServer::Instance()->SetupFramePasses();
		//Always setup shaders before materials!
		ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
		//Load all materials
		ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");
	    //Init debugrenderer. Always do this AFTER setting up shaders!
        Debug::DebugRenderer::Instance()->Initialize();


		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1050);
		this->window->SetTitle("Vortex Engine Test Environment");

		this->consoleBuffer = new char[CONSOLE_BUFFER_SIZE];
		
		// Load Scene
		modelInstanceScene = std::make_shared<Render::ModelInstance>();
		modelInstanceScene->SetSurface("resources/surfaces/rock.surface");
		modelInstanceScene->SetMesh("resources/models/groundfloor.obj");

		SceneEntity1 = std::make_shared<Game::StaticEntity>();
        SceneEntity2 = std::make_shared<Game::StaticEntity>();
        SceneEntity3 = std::make_shared<Game::StaticEntity>();
        SceneEntity4 = std::make_shared<Game::StaticEntity>();
        SceneEntity5 = std::make_shared<Game::StaticEntity>();
        SceneEntity6 = std::make_shared<Game::StaticEntity>();

		SceneEntity1->SetModel(modelInstanceScene);
		SceneEntity2->SetModel(modelInstanceScene);
		SceneEntity3->SetModel(modelInstanceScene);
		SceneEntity4->SetModel(modelInstanceScene);
		SceneEntity5->SetModel(modelInstanceScene);
		SceneEntity6->SetModel(modelInstanceScene);

		SceneEntity1->SetTransform(Math::mat4::translation(0.0f, -2.0f, 0.0f));
        SceneEntity2->SetTransform(Math::mat4::multiply(Math::mat4::rotationz(1.57f), Math::mat4::translation(10.0f, 8.0f, 0.0f)));
        SceneEntity3->SetTransform(Math::mat4::multiply(Math::mat4::rotationz(1.57f), Math::mat4::translation(-10.0f, 8.0f, 0.0f)));
        SceneEntity4->SetTransform(Math::mat4::multiply(Math::mat4::rotationx(1.57f), Math::mat4::translation(0.0f, 8.0f, 10.0f)));
        SceneEntity5->SetTransform(Math::mat4::multiply(Math::mat4::rotationx(1.57f), Math::mat4::translation(0.0f, 8.0f, -10.0f)));
        SceneEntity6->SetTransform(Math::mat4::translation(0.0f, 18.0f, 0.0f));
		
		SceneEntity1->Activate();
        SceneEntity2->Activate();
        SceneEntity3->Activate();
        SceneEntity4->Activate();
        SceneEntity5->Activate();
        SceneEntity6->Activate();

		modelInstance = std::make_shared<Render::ModelInstance>();
		modelInstance1 = std::make_shared<Render::ModelInstance>();

		rigidBodyEntity1 = std::make_shared<Game::StaticEntity>();
		rigidBodyEntity2 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity3 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity4 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity5 = std::make_shared<Game::RigidBodyEntity>();

		modelInstance->SetSurface("resources/surfaces/player.surface");
		modelInstance->SetMesh("resources/models/kung.obj");

		modelInstance1->SetSurface("resources/surfaces/placeholder.surface");
		modelInstance1->SetMesh("resources/models/cube.obj");

		PointLight pLight;
		pLight.position = Math::vec4(3.0f, 2.0f, 1.0f, 1.0f);
		pLight.color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pLight.radiusAndPadding.set_x(10.0f);
		LightServer::Instance()->AddPointLight(pLight);
		
		pLight.position = Math::vec4(3.0f, 2.0f, 5.0f, 1.0f);
		pLight.color = Math::vec4(0.7f, 0.5f, 0.3f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(8.0f, -1.5f, 5.0f, 1.0f);
		pLight.color = Math::vec4(0.3f, 0.5f, 0.7f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(10.0f, -1.5f, 10.0f, 1.0f);
		pLight.color = Math::vec4(0.1f, 1.0f, 0.1f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		rigidBodyEntity1->SetModel(modelInstance1);
		rigidBodyEntity2->SetModel(modelInstance1);
		rigidBodyEntity3->SetModel(modelInstance1);
		rigidBodyEntity4->SetModel(modelInstance1);
		rigidBodyEntity5->SetModel(modelInstance);

		Math::mat4 transf1 = Math::mat4::translation(2.0f, -0.5f, 1.5f);
        Math::mat4 transf2 = Math::mat4::translation(0.0f, -0.5f, 1.5f);
		Math::mat4 transf3 = Math::mat4::translation(2.0f, -0.5f, -1.5f);
		Math::mat4 transf4 = Math::mat4::translation(0.0f, -0.5f, -1.5f);
        Math::mat4 transf5 = Math::mat4::translation(0.0f, -0.5f, -3.0f);

        this->rigidBodyEntity1->SetTransform(transf1);
		this->rigidBodyEntity2->SetTransform(transf2);
		this->rigidBodyEntity3->SetTransform(transf3);
		this->rigidBodyEntity4->SetTransform(transf4);
        this->rigidBodyEntity5->SetTransform(transf5);

        rigidBodyEntity1->Activate();
        rigidBodyEntity2->Activate();
        rigidBodyEntity3->Activate();
        rigidBodyEntity4->Activate();
        rigidBodyEntity5->Activate();

        // set ui rendering function
		this->window->SetUiRender([this]()
		  {
			  this->RenderUI();
		  });

		this->window->SetNanoVGRender([this](NVGcontext * vg)
		  {
			  this->RenderNano(vg);
		  });

		return true;
	}
	return false;
}

void ExampleApp::RenderUI()
{
	if (this->window->IsOpen())
	{
		static bool show = true;
		// create a new window
		ImGui::Begin("Console", &show, ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowSize(ImVec2(450.0f,210.0f), ImGuiSetCond_::ImGuiSetCond_Once);
		ImGui::Text("Frame time: %f\n", this->frameTime);
		currentFPS = 1.0f / this->frameTime;		
		ImGui::Text("FPS: %f\n", currentFPS);
		ImGui::End();
	}
}

void ExampleApp::RenderNano(NVGcontext * vg)
{
	nvgSave(vg);
	nvgRestore(vg);
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	// set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	while (this->window->IsOpen())
	{
		double time = glfwGetTime();
		this->window->Update();

		Physics::PhysicsDevice::Instance()->Solve();
		BaseGameFeature::EntityManager::Instance()->Update();

		if (ImGui::GetIO().KeyAlt)
		{
			CameraMovement();
		}

		Debug::DebugRenderer::Instance()->DrawLine(this->rayStart, this->rayEnd, 2.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		RenderDevice::Instance()->Render();

		this->window->SwapBuffers();

		this->frameTime = glfwGetTime() - time;

	}
}


void ExampleApp::CameraMovement()
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
void ExampleApp::DoPicking()
{
	ImVec2 dockPos = ImGui::GetWindowPos();

	ImGuiStyle& style = ImGui::GetStyle();
	dockPos.x += style.WindowPadding.x;
	dockPos.y += style.WindowPadding.y;

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

		Physics::PhysicsHit newHit;
		if (Physics::PhysicsServer::Instance()->Raycast(newHit, rayWorldPos, rayDirection, 400.0f))
		{
			this->rayStart = rayWorldPos;

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

} // namespace Example
