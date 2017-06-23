//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <cstring>
#include "imgui.h"
#include "IO/console.h"
#include "ImGUIExtra.h"
#include "application/basegamefeature/managers/scenemanager.h"
#include "render/server/resourceserver.h"

using namespace Display;

namespace ParticleEditor
{

//------------------------------------------------------------------------------
/**
*/
Application::Application() :
	shutdown(false)
{
	
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

#ifdef _DEBUG
	//Open Console
	IO::Console::Instance()->Show();
	IO::Console::Instance()->OpenNativeConsole();
#endif

	//Always call app::open _AFTER_ initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);
	
		Render::RenderDevice::Instance()->Initialize();
		Render::RenderDevice::Instance()->SetRenderResolution({ 1920, 1200 });
		
		UI = std::make_shared<UserInterface>(std::shared_ptr<ParticleEditor::Application>(this));

		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Particle Editor");

		//spawn in a cube somewhere
		this->wall1 = Game::ModelEntity::Create();
		this->wall1->SetModel(Render::ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));
		this->wall1->Activate();
		this->wall1->SetTransform(Math::mat4::translation(0.0f, 0.0f, -2.0f));

		this->billboard = Game::ParticleEntity::Create();
		this->billboard->SetTransform(Math::mat4::translation(0.f, -0.1f, -1.f));
		this->billboard->LoadEmitters("resources/particles/wisp.particle");
		this->billboard->Activate();

		for (size_t i = 0; i < this->billboard->GetEmitters().Size(); i++)
		{
			this->particleList.Append(this->billboard->GetEmitters()[i]);
		}

		ImGui::SetupImGuiStyle(true, 1.0f);

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

		//Imgui code goes here!
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		BaseGameFeature::EntityManager::Instance()->Update();
		//Application loop

		Render::RenderDevice::Instance()->Render(false);
		this->window->SwapBuffers();
	}

	this->window->Close();
}

} // namespace ParticleEditor
