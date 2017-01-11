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
	App::Open();
	this->window = new Display::Window;
	
	keyhandler = BaseGameFeature::KeyHandler::Instance();
	keyhandler->Init(this->window);
	
	this->rayStart = Math::vec4::zerovector();
	this->rayEnd = Math::vec4::zerovector();
	hit.object = nullptr;

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
		//Setup UI
		this->UI = new UserInterface(this);


		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Vortex Level Editor");

		this->rayStart = Math::vec4::zerovector();
		this->rayEnd = Math::vec4::zerovector();
		
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

		rigidBodyEntity1 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity2 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity3 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity4 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity5 = std::make_shared<Game::RigidBodyEntity>();
		
		modelInstance->SetSurface("resources/surfaces/player.surface");
		modelInstance->SetMesh("resources/models/kung.obj");
		
		modelInstance1->SetSurface("resources/surfaces/placeholder.surface");
		modelInstance1->SetMesh("resources/models/cube.obj");


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

        const int numEntsX = 1;
        const int numEntsY = 1;

        for (int i = 0; i < numEntsX; ++i) {
            for (int j = 0; j < numEntsY; ++j) {
                std::shared_ptr<Game::RigidBodyEntity> ent = std::make_shared<Game::RigidBodyEntity>();
                RBEs.Append(ent);
				ent->SetModel(modelInstance1);

                Math::mat4 transf1 = Math::mat4::translation(i*1.001f - (numEntsX/2), numEntsY/4 + j*1.001f, 5.0f);
                ent->SetTransform(transf1);
                ent->Activate();
            }
        }

        //rigidBodyEntity1->Activate();
        //rigidBodyEntity2->Activate();
        //rigidBodyEntity3->Activate();
        //rigidBodyEntity4->Activate();
        //rigidBodyEntity5->Activate();

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

		RenderDevice::Instance()->Render(false);

		this->window->SwapBuffers();
		UI->frameTime = glfwGetTime() - time;
	}

	this->window->Close();
}

} // namespace LevelEditor
