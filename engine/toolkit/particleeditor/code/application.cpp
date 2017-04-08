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

	//Open Console
	IO::Console::Instance()->Show();
	IO::Console::Instance()->OpenNativeConsole();

	//Always call app::open _AFTER_ initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);
		
		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Particle Editor");

		ImGui::SetupImGuiStyle(true, 1.0f);

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
		//Updates the console. Always do this first!
		IO::Console::Instance()->Update();

		//Imgui code goes here!
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
    float a = 0.0f;
	while (this->window->IsOpen() && !this->shutdown)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
       	
		//Application loop

		this->window->SwapBuffers();
	}

	this->window->Close();
}

} // namespace ParticleEditor
