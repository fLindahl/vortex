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
#include "application/basegamefeature/managers/envmanager.h"
#include "IO/console.h"

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
	this->renderGeoProxies = false;

	this->commandManager = Edit::CommandManager::Instance();
	cameraPos = Math::point::zerovector();
	camRotX = 0;
	camRotY = 0;

	this->rayStart = Math::vec4::zerovector();
	this->rayEnd = Math::vec4::zerovector();
	this->reflectStart = Math::vec4::zerovector();
	this->reflectEnd = Math::vec4::zerovector();
	hit.object = nullptr;

	this->pickingPixels = new GLuint[1920 * 1020];
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
	RenderDevice::Instance()->SetPickingEnabled(true);

	IO::Console::Instance()->Show();
	IO::Console::Instance()->OpenNativeConsole();

	//Always call app::open _AFTER_ initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);

		//Init RenderDevice
		Render::RenderDevice::Instance()->Initialize();

		//Setup UI
		this->UI = new UserInterface(this);
		
		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Vortex Level Editor");

		RenderDevice::Instance()->SetRenderResolution(1920, 1020);

		//this->rayStart = Math::vec4::zerovector();
		//this->rayEnd = Math::vec4::zerovector();
		
		// set ui rendering function
		this->window->SetUiRender([this]()
		  {
			  this->RenderUI();
		  });

		return true;
	}
	return false;
}

void Application::RenderUI()
{
	if (this->window->IsOpen())
	{	
		//Updates the console. Always do this first!
		IO::Console::Instance()->Update();

		UI->Run();		
	}
}

//------------------------------------------------------------------------------
/**
*/
void
Application::Run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    float a = 0.0f;
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

		if (this->hit.object != nullptr)
		{
			Game::PhysicsEntity* e = dynamic_cast<Game::PhysicsEntity*>(hit.object);
			if (e != nullptr)
			{
				Debug::DebugRenderer::Instance()->DrawMesh(e->GetGraphicsProperty()->getModelInstance()->GetMesh(), e->GetTransform(), Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), Debug::RenderMode::WireFrame, -1, 2.0f);
			}
		}

		if (renderGeoProxies)
		{
			BaseGameFeature::EnvManager::Instance()->RenderGeometryProxies();
		}

		RenderDevice::Instance()->Render(false);

		this->window->SwapBuffers();
		UI->frameTime = glfwGetTime() - time;
	}

	this->window->Close();
}

} // namespace LevelEditor
