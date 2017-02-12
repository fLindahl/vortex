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
#include "application/properties/particleemitter.h"
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

		//RenderDevice::Instance()->SetRenderResolution(256, 256);

		this->rayStart = Math::vec4::zerovector();
		this->rayEnd = Math::vec4::zerovector();
		
		//Load Sponza
		this->sponza = std::make_shared<Game::ModelEntity>();
		this->sponza->SetModel(ResourceServer::Instance()->LoadModel("resources/models/sponza.mdl"));
		this->sponza->Activate();
		Math::mat4 sTransform = Math::mat4::scaling(0.01f, 0.01f, 0.01f);
		sTransform.translate(Math::vector(0.0f, -2.0f, 0.0f));

		this->sponza->SetTransform(sTransform);


		billboard = std::make_shared<Game::ParticleEntity>();
		billboard->SetTransform(Math::mat4::translation(11.18f, -1.0f, 4.02f));
		billboard->Activate();
		billboard->GetEmitter()->CreateEmitter(1000, "resources/textures/particles/sprite_rapids2.tga");

		billboard2 = std::make_shared<Game::ParticleEntity>();
		billboard2->SetTransform(Math::mat4::translation(0.0f, 5.5f, 0.0f));
		billboard2->Activate();
		billboard2->GetEmitter()->CreateEmitter(20000, "resources/textures/particles/fireparticle3.tga");
		
		particleList.Append(billboard);
		particleList.Append(billboard2);

		PointLight pLight;
		pLight.position = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pLight.color = Math::vec4(1.0f, 0.5f, 1.0f, 1.0f);
		pLight.radiusAndPadding.set_x(10.0f);
		LightServer::Instance()->AddPointLight(pLight);
		
		pLight.position = Math::vec4(5.0f, 1.0f, 3.0f, 1.0f);
		pLight.color = Math::vec4(0.7f, 0.0f, 0.3f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(2.0f, -1.0f, -0.0f, 1.0f);
		pLight.color = Math::vec4(0.3f, 0.5f, 0.7f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(0.0f, -1.5f, 0.0f, 1.0f);
		pLight.color = Math::vec4(0.1f, 0.5f, 0.1f, 1.0f);
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
		
		//Debug::DebugRenderer::Instance()->DrawCircle(Math::point(0, 0, 0), Math::quaternion::identity(), 0.5f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::Normal, 2.0f);

		//Debug::DebugRenderer::Instance()->DrawCone(Math::point(2, 0, 0), Math::quaternion::rotationyawpitchroll(0.0f, 3.14f, 45.0f), 0.5f, 1.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::Normal, 2.0f);

		//Debug::DebugRenderer::Instance()->DrawCircle(Math::point(0, 0, 0), Math::quaternion::identity(), 0.5f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::Normal, 2.0f);

		if (this->hit.object != nullptr)
		{
			Game::PhysicsEntity* e = dynamic_cast<Game::PhysicsEntity*>(hit.object);
			if (e != nullptr)
			{
				Debug::DebugRenderer::Instance()->DrawMesh(e->GetGraphicsProperty()->getModelInstance()->GetMesh(), e->GetTransform(), Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), Debug::RenderMode::WireFrame, -1, 2.0f);
			}
		}
		
		RenderDevice::Instance()->Render(false);

		this->window->SwapBuffers();
		UI->frameTime = glfwGetTime() - time;
	}

	this->window->Close();
}

} // namespace LevelEditor
