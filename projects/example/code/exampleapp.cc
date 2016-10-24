//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <render/server/renderdevice.h>

#include "application/basegamefeature/keyhandler.h"

using namespace Display;
using namespace Render;

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

		return true;
	}
	return false;
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