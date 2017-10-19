//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <physics/physicsdevice.h>
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
#include "selecttool.h"
#include "physics/boxcolliderproperty.h"

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
	//hit.object = nullptr;

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
		_printf("Render Resolution is %i*%i.", RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y);

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

	//Ptr<Game::Entity> entity = Game::Entity::Create();
	//Ptr<Render::GraphicsProperty> gProperty = Render::GraphicsProperty::Create();
	//entity->AddProperty(gProperty.upcast<Game::BaseProperty>());
	//gProperty->setModelInstance(ResourceServer::Instance()->LoadModel("resources/models/sponza.mdl"));
	//
	//entity->Activate();

	Ptr<Game::Entity> physicsEntity = Game::Entity::Create();
	Ptr<Property::BoxCollider> collider = Property::BoxCollider::Create();
	physicsEntity->AddProperty(collider.upcast<Game::BaseProperty>());
	Ptr<Property::Rigidbody> rbp = Property::Rigidbody::Create();
	physicsEntity->AddProperty(rbp.upcast<Game::BaseProperty>());
	Ptr<Render::GraphicsProperty> gProperty2 = Render::GraphicsProperty::Create();
	physicsEntity->AddProperty(gProperty2.upcast<Game::BaseProperty>());
	gProperty2->setModelInstance(ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));
	physicsEntity->SetTransform(Math::mat4::translation(0, 0.0f, -3));	
	physicsEntity->Activate();

	Ptr<Game::Entity> physicsEntity2 = Game::Entity::Create();
	Ptr<Property::BoxCollider> collider2 = Property::BoxCollider::Create();
	collider2->SetSize(Math::vector(20, 0.5f, 20));
	physicsEntity2->AddProperty(collider2.upcast<Game::BaseProperty>());
	Ptr<Render::GraphicsProperty> gProperty3 = Render::GraphicsProperty::Create();
	physicsEntity2->AddProperty(gProperty3.upcast<Game::BaseProperty>());
	gProperty3->setModelInstance(ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));

	physicsEntity2->Activate();
	physicsEntity2->SetTransform(Math::mat4::multiply(Math::mat4::translation(0, -2.0f, 0), Math::mat4::scaling(20, 0.5f, 20)));



	LightServer::Instance()->CreatePointLight(Math::point(1, 1, 1), Math::point(0, 0, 3), 15.0f);

	Tools::ToolHandler::Instance()->SelectTool()->SetSelectedEntity(physicsEntity);

	while (this->window->IsOpen() && !this->shutdown)
	{
		double time = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
       	
		if (ImGui::GetIO().MouseClicked[1])
			Physics::PhysicsDevice::Instance()->StepSimulation(1.f / 10.f);

		//for (auto line : Physics::PhysicsDevice::Instance()->visualDebug)
		//	Debug::DebugRenderer::Instance()->DrawLine(line.start(),line.end(),1.0f,Math::vec4(1,0,0,1),Math::vec4(1,0,0,0.1f), Debug::RenderMode::AlwaysOnTop);

		BaseGameFeature::EntityManager::Instance()->Update();

		if (ImGui::GetIO().KeyAlt)
		{
			CameraMovement();
		}

		/*
		if (this->hit.object != nullptr)
		{
			Ptr<Render::GraphicsProperty> gp = this->hit.object->FindProperty<Render::GraphicsProperty>();
			if (gp.isvalid())
			{
				Debug::DebugRenderer::Instance()->DrawMesh(gp->getModelInstance()->GetMesh(), this->hit.object->GetTransform(), Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), Debug::RenderMode::WireFrame, -1, 2.0f);
			}
		}
		*/

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
