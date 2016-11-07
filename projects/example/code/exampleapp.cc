//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include "imgui.h"

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
	App::Open();
	this->window = new Display::Window;

	
	keyhandler = BaseGameFeature::KeyHandler::Instance();
	keyhandler->Init(this->window);

	// Initiate everything we need
	// TODO: We should be able to cut down on a lot of this code by creating our own resource structures
	if (this->window->Open())
	{
		this->consoleBuffer = new char[CONSOLE_BUFFER_SIZE];

		modelInstance = std::make_shared<Render::ModelInstance>();
		modelInstance1 = std::make_shared<Render::ModelInstance>();

		gProperty = new Render::GraphicsProperty();
		gProperty1 = new Render::GraphicsProperty();

		//Always setup shaders before materials!
		ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
		//Load all materials
		ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");
		
		modelInstance->SetMaterial("Static");
		modelInstance->SetMesh("resources/models/player.nvx2");
		gProperty->setModelInstance(modelInstance);
		
		modelInstance1->SetMaterial("Toon");
		modelInstance1->SetMesh("resources/models/player.nvx2");
		gProperty1->setModelInstance(modelInstance1);

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
		bool show = true;
		// create a new window
		ImGui::Begin("Console", &show, ImGuiWindowFlags_NoSavedSettings);

		// create text editors for shader code
		ImGui::InputTextMultiline("Vertex Shader", consoleBuffer, CONSOLE_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
								  ImGuiInputTextFlags_AllowTabInput);

		//ImGui::InputTextMultiline("Pixel Shader", fsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
		//						  ImGuiInputTextFlags_AllowTabInput);

		// apply button
		if (ImGui::Button("Apply"))
		{
			// if pressed we compile the shaders
			//	this->CompileShaders();
		}
		//if (this->compilerLog.length())
		//{
			// if compilation produced any output we display it here
		//	ImGui::TextWrapped(this->compilerLog.c_str());
		//}
		// close window
		ImGui::End();
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

	int xangle = 0;
	int yangle = 0;
	float radians = 0.0f;
	bool decrease = false;
	float speed = 0.1f;

	float x = 0.0f;
	float y = 0.0f;
	//float z = 0.0f;

	while (this->window->IsOpen())
	{
		this->window->Update();

		// Rotation and translation
		Math::Matrix4 transf = Math::Matrix4::translation(0.0f, 0.0f, 0.0f) * Math::Matrix4::rotY((float)keyhandler->mouseY) * Math::Matrix4::rotX((float)keyhandler->mouseX) * Math::Matrix4::translation(x, y, (float)keyhandler->scroll * -0.1f - 1.0f);
		Math::Matrix4 transf2 = Math::Matrix4::translation(1.0f, 0.0f, 0.0f) * Math::Matrix4::rotY((float)keyhandler->mouseY) * Math::Matrix4::rotX((float)keyhandler->mouseX) * Math::Matrix4::translation(x, y, (float)keyhandler->scroll * -0.1f - 1.0f);
		
		gProperty->setModelMatrix(transf);
		gProperty1->setModelMatrix(transf2);

		RenderDevice::Instance()->Render();

		this->window->SwapBuffers();
	}
}

} // namespace Example