//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>

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


	//Setup keyboard and mouse functions
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32){
		if (action == GLFW_PRESS)
			keyhandler.setKeyStatePressed(key);
		else if (action == GLFW_RELEASE)
			keyhandler.setKeyStateReleased(key);
	});
	window->SetMousePressFunction([this](int32 key, int32 action, int32){
		if (action == GLFW_PRESS)
			keyhandler.setKeyStatePressed(key);
		if (action == GLFW_RELEASE)
			keyhandler.setKeyStateReleased(key);
	});
	window->SetMouseMoveFunction([this](float64 y, float64 x){
		if (keyhandler.leftMousePressed == true){
			this->mouseX += x - oldx;
			this->mouseY += y - oldy;
		}
		if (keyhandler.rightMousePressed == true){
			this->lightX += x - oldx;
			this->lightY += y - oldy;
		}
		oldx = x;
		oldy = y;
	});
	window->SetMouseScrollFunction([this](float64, float64 y){
		if (keyhandler.rightMousePressed == true)
			this->lightZ += y;
		else
			this->z += y;
	});

	// Initiate everything we need
	// TODO: We should be able to cut down on a lot of this code by creating our own resource structures
	if (this->window->Open())
	{
		mesh = std::make_shared<Render::MeshResource>();
		shader = std::make_shared<Render::ShaderObject>();
		shader1 = std::make_shared<Render::ShaderObject>();
		texture = std::make_shared<Render::TextureResource>();
		modelInstance = std::make_shared<Render::ModelInstance>();
		modelInstance1 = std::make_shared<Render::ModelInstance>();
		gProperty = new Render::GraphicsProperty();
		gProperty1 = new Render::GraphicsProperty();

		// Load our shaders
		shader->loadVertexShader("resources/shaders/vertex.vert");
		shader->loadFragmentShader("resources/shaders/phong.frag");
		shader->LinkShaders();

		// Because we're trying to load the same vertex shader twice, the shaderserver will return the same shader as before, thus not needing to recompile it
		shader1->loadVertexShader("resources/shaders/vertex.vert");
		shader1->loadFragmentShader("resources/shaders/toonshader.frag");
		shader1->LinkShaders();

		modelInstance->setShaderObject(shader);
		modelInstance->setMesh(mesh);
		modelInstance->setTexture(texture);
		gProperty->setModelInstance(modelInstance);

		modelInstance1->setShaderObject(shader1);
		modelInstance1->setMesh(mesh);
		modelInstance1->setTexture(texture);
		gProperty1->setModelInstance(modelInstance1);

		mesh->loadMeshFromFile("resources/models/cat.obj");
		mesh->setupMesh();

		texture->loadFromFile("resources/textures/cat_diff.tga");

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

		if (keyhandler.tUP == true)
			y = y + speed;
		if (keyhandler.tDOWN == true)
			y = y - speed;
		if (keyhandler.tLEFT == true)
			x = x - speed;
		if (keyhandler.tRIGHT == true)
			x = x + speed;
		if (keyhandler.quit == true)
			this->window->Close();

		// Rotation och translation med mus och tangentbord f�r model
		Math::Matrix4 transf = Math::Matrix4::translation(0.0f, 0.0f, 0.0f) * Math::Matrix4::rotY((float)mouseY) * Math::Matrix4::rotX((float)mouseX) * Math::Matrix4::translation(x, y, (float)z * -0.1f - 1.0f);

		Math::Matrix4 transf2 = Math::Matrix4::translation(1.0f, 0.0f, 0.0f) * Math::Matrix4::rotY((float)lightY) * Math::Matrix4::rotX((float)lightX) * Math::Matrix4::translation(x, y, (float)lightZ * -0.1f - 1.0f);
		
		gProperty->setModelMatrix(transf);
		gProperty1->setModelMatrix(transf2);

		GraphicsServer::Instance()->Render();

		this->window->SwapBuffers();
	}
}

} // namespace Example