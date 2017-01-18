//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <fysik/physicsdevice.h>
#include <render/debugrender/debugrenderer.h>
#include "foundation/math/plane.h"
#include "imgui.h"
#include "render/server/frameserver.h"
#include "render/server/lightserver.h"
#include "application/basegamefeature/managers/scenemanager.h"

#include "application/basegamefeature/keyhandler.h"

using namespace Display;
using namespace Render;
using namespace Toolkit;

namespace LevelEditor
{

//------------------------------------------------------------------------------
/**
*/
Application::Application()
{
	this->commandManager = Edit::CommandManager::Instance();
	cameraPos = Math::point::zerovector();
	camRotX = 0;
	camRotY = 0;

	this->rayStart = Math::vec4::zerovector();
	this->rayEnd = Math::vec4::zerovector();
	hit.object = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
Application::~Application()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
Application::Open()
{
	this->window = new Display::Window;
	// Initiate everything we need
	//Always call app::open after initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);
		//Setup UI
		this->UI = new UserInterface(this);
		
		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Vortex Level Editor");

		this->rayStart = Math::vec4::zerovector();
		this->rayEnd = Math::vec4::zerovector();
		
		//Load Sponza
		//this->sponza = std::make_shared<Game::ModelEntity>();
		//this->sponza->SetModel(BaseGameFeature::SceneManager::Instance()->LoadOBJScene("resources/scenes/sponza.obj", "resources/scenes/sponza.mtl"));
		//this->sponza->Activate();
		//this->sponza->SetTransform(Math::mat4::scaling(0.2f, 0.2f, 0.2f));

		// Load Scene
		modelInstanceScene = ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl");

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
		
		modelInstance = ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl");

		rigidBodyEntity1 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity2 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity3 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity4 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity5 = std::make_shared<Game::RigidBodyEntity>();

		rigidBodyEntity1->SetModel(modelInstance);
		rigidBodyEntity2->SetModel(modelInstance);
		rigidBodyEntity3->SetModel(modelInstance);
		rigidBodyEntity4->SetModel(modelInstance);
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

void Application::RenderUI()
{
	if (this->window->IsOpen())
	{		
		UI->Run();		
	}
}

void Application::RenderNano(NVGcontext * vg)
{
	nvgSave(vg);

	/*
	nvgBeginPath(vg);
	nvgCircle(vg,600, 100, 50);
	NVGpaint paint;
	paint = nvgLinearGradient(vg, 600, 100, 650, 150, nvgRGBA(255, 0, 0, 255), nvgRGBA(0, 255, 0, 255));
	nvgFillPaint(vg, paint);
	nvgFill(vg);

	*/
	nvgRestore(vg);
}

//------------------------------------------------------------------------------
/**
*/
void
Application::Run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (this->window->IsOpen() && !this->shutdown)
	{
		double time = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
       	
		Physics::PhysicsDevice::Instance()->Solve();
		BaseGameFeature::EntityManager::Instance()->Update();

		if (ImGui::GetIO().KeyAlt)
		{
			CameraMovement();
		}

		Debug::DebugRenderer::Instance()->DrawLine(this->rayStart, this->rayEnd, 2.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Debug::DebugRenderer::Instance()->DrawCone(Math::point(0, 0, 0), Math::quaternion::rotationyawpitchroll(0.0f, 3.14f, 0.0f), 0.5f, 1.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::Normal, 2.0f);

		//Debug::DebugRenderer::Instance()->DrawCircle(Math::point(0, 0, 0), Math::quaternion::identity(), 0.5f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::Normal, 2.0f);

		if (this->hit.object != nullptr)
		{
			Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(hit.object);
			if (rbe != nullptr)
			{
				Debug::DebugRenderer::Instance()->DrawMesh(rbe->GetGraphicsProperty()->getModelInstance()->GetMesh(), rbe->GetTransform(), Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), Debug::RenderMode::WireFrame, -1, 2.0f);
			}
		}
		
		RenderDevice::Instance()->Render(false);

		this->window->SwapBuffers();
		UI->frameTime = glfwGetTime() - time;
	}

	this->window->Close();
}

} // namespace LevelEditor
