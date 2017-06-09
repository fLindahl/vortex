//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "imgui.h"
#include "imgui_dock.h"
#include "IO/console.h"
#include "render/server/renderdevice.h"
#include "application/game/modelentity.h"
#include "render/server/resourceserver.h"
#include "render/properties/graphicsproperty.h"

namespace Example
{
	//----------------------
	//TEST CLASS
	__ImplementClass(Example::TestClass, 'TeSt', Core::RefCounted);
	TestClass::TestClass() : value(0)
	{
	
	}
	TestClass::~TestClass()
	{

	}



//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
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
	this->window = new Display::Window;
	// Initiate everything we need
	//Always call app::open after initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);
		
		//Init RenderDevice
		Render::RenderDevice::Instance()->Initialize();

		//Init resize callback for renderdevice.
		//Do this before setting window size.
		this->window->SetWindowResizeFunction([this](int32 x, int32 y)
		{
			this->ResizeWindow(x, y);
		});

		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(500, 500);
		this->window->SetTitle("Vortex Engine Test Environment");

		
        // set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});

		Render::RenderDevice::Instance()->SetWindowResolution(500, 500);
		Render::RenderDevice::Instance()->SetRenderResolution(800, 600);

		return true;
	}
	return false;
}

void ExampleApp::RenderUI()
{
	if (this->window->IsOpen())
	{
		ImGui::RootDock(ImVec2(0, 0), ImVec2(window->GetWidth(), window->GetHeight()));
		{
			if(ImGui::BeginDock("TestDock1", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
			{
				ImGui::Text("Testing Dock 1");

			}
			ImGui::EndDock();

			if(ImGui::BeginDock("TestDock2", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
			{
				ImGui::Text("Testing Dock 2");

			}
			ImGui::EndDock();

		}
		
		//Updates the console. Always do this first!
		IO::Console::Instance()->Update();
	}
}


void ExampleApp::ResizeWindow(int32 x, int32 y)
{
	Render::RenderDevice::Instance()->SetWindowResolution(x, y);
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	// set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	IO::Console::Instance()->OpenNativeConsole();
	IO::Console::Instance()->Show();

	Ptr<TestClass> test;
	if (Core::Factory::Instance()->ClassExists("Example::ANonExistingClass"))
	{
		Ptr<Core::RefCounted> ref = Core::Factory::Instance()->Create("Example::ANonExistingClass");
		test = ref.downcast<TestClass>();
	}
	else
	{
		_warning("Could not find Example::ANonExistingClass");		
	}

	if (Core::Factory::Instance()->ClassExists("Example::TestClass"))
	{
		_printf("Found class named Example::TestClass");
		Ptr<Core::RefCounted> ref = Core::Factory::Instance()->Create("Example::TestClass");
		test = ref.downcast<TestClass>();
	}

	Ptr<TestClass> test2 = TestClass::Create();

	IO::Console::Instance()->Print("This is a message!", IO::MESSAGE);
	IO::Console::Instance()->Print("This is an input!", IO::INPUT);
	IO::Console::Instance()->Print("This is a warning!", IO::WARNING);
	IO::Console::Instance()->Print("This is an error!", IO::ERROR);
	IO::Console::Instance()->Print("This is an exception!", IO::EXCEPTION);

	Ptr<Render::GraphicsProperty> gp = Render::GraphicsProperty::Create();

	IO::Stream* stream = new IO::Stream();

	gp->Serialize(stream);
	gp->Serialize(stream);

	gp->integerAttr.value = 10;

	gp->Serialize(stream);

	while (this->window->IsOpen())
	{
		//Do this before anything else
		this->window->MakeCurrent();
		//This polls for events
		this->window->Update();
		
		//Render the scene
		Render::RenderDevice::Instance()->Render();

		//Present Screen
		this->window->SwapBuffers();	
	}
}


} // namespace Example
