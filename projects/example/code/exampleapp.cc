//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "imgui.h"
#include "IO/console.h"

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
	
		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1600, 900);
		this->window->SetTitle("Vortex Engine Test Environment");

		Render::RenderDevice::Instance()->SetRenderResolution(1600, 900);

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
		//Updates the console. Always do this first!
		IO::Console::Instance()->Update();
	}
}

void ExampleApp::RenderNano(NVGcontext * vg)
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
ExampleApp::Run()
{
	// set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
	
	while (this->window->IsOpen())
	{
		//Do this before anything else
		//This polls for events
		this->window->Update();
			
		Render::RenderDevice::Instance()->Render();

		//Present Screen
		this->window->SwapBuffers();
	}
}


} // namespace Example
