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

		// Load our shaders
		ShaderServer::Instance()->LoadShader("resources/shaders/vertex.vert");
		ShaderServer::Instance()->LoadShader("resources/shaders/phong.frag");
		ShaderServer::Instance()->LoadShader("resources/shaders/toonshader.frag");
	
        // Because we're trying to load the same vertex shader twice, the shaderserver will return the same shader as before, thus not needing to recompile it
        // If we we're to ty to load the same shader object twice it's the same principle.
        shader = ShaderServer::Instance()->LoadShaderObject("resources/shaders/vertex.vert", "resources/shaders/phong.frag");
        shader1 = ShaderServer::Instance()->LoadShaderObject("resources/shaders/vertex.vert", "resources/shaders/toonshader.frag");

		modelInstance->setShaderObject(shader);
		modelInstance->setMesh("resources/models/player.nvx2");
		modelInstance->setTexture("resources/textures/player.png");
		gProperty->setModelInstance(modelInstance);

		modelInstance1->setShaderObject(shader1);
		modelInstance1->setMesh("resources/models/player.nvx2");
		modelInstance1->setTexture("resources/textures/player.png");
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