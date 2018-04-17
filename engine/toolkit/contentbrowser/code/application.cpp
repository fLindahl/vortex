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
#include "tinyxml2.h"
#include "render/resources/surface.h"
#include "render/resources/meshbuilder.h"

#include "application/basegamefeature/keyhandler.h"

using namespace Display;
using namespace Render;
using namespace Toolkit;

namespace ContentBrowser
{

//------------------------------------------------------------------------------
/**
*/
Application::Application()
{
	cameraPos = Math::point::zerovector();
	camRotX = 0;
	camRotY = 0;
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
	//Always call app::open after initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);

		//Init RenderDevice
		Render::RenderDevice::Instance()->Initialize();

		//Setup UI
		this->UI = new UserInterface(this);

		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(2320, 1080);
		this->window->SetTitle("Vortex Content Browser");

		Render::RenderDevice::Instance()->SetRenderResolution(1920, 1080);

		LightServer::PointLight pLight;
		pLight.position = Math::vec4(3.0f, 2.0f, 1.0f, 1.0f);
		pLight.color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pLight.radiusAndPadding.set_x(10.0f);
		LightServer::Instance()->AddPointLight(pLight);
		
		pLight.position = Math::vec4(3.0f, 2.0f, 5.0f, 1.0f);
		pLight.color = Math::vec4(0.7f, 0.5f, 0.3f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(8.0f, -1.5f, 5.0f, 1.0f);
		pLight.color = Math::vec4(0.3f, 0.5f, 0.7f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(10.0f, -1.5f, 10.0f, 1.0f);
		pLight.color = Math::vec4(0.1f, 1.0f, 0.1f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);
		
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

	while (this->window->IsOpen() && !this->shutdown)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
       	
		//Physics::PhysicsDevice::Instance()->Solve();
		BaseGameFeature::EntityManager::Instance()->Update();

		if (ImGui::GetIO().KeyAlt)
		{
			CameraMovement();
		}

		RenderDevice::Instance()->Render(false);

		if (this->UI->selectedNode != nullptr)
		{
			Debug::DebugRenderer::Instance()->DrawMesh(this->entity->FindProperty<GraphicsProperty>()->getModelInstance()->GetMesh(), this->entity->GetTransform(), Math::vec4(0.5f, 0.5f, 0.0f, 0.5f), (Debug::RenderMode)(Debug::RenderMode::WireFrame | Debug::RenderMode::AlwaysOnTop), this->UI->selectedNode->primitiveGroup, 1.0f);
		}

		this->window->SwapBuffers();
	}

	this->window->Close();
}

void Application::NewModel()
{
	if (this->entity != nullptr)
	{
		this->entity->Deactivate();
	}

	this->entity = Game::Entity::Create();
	Ptr<GraphicsProperty> gp = GraphicsProperty::Create();
	this->entity->AddProperty(gp.downcast<Game::BaseProperty>());

	//Ptr<Render::ModelInstance> mdlInst = std::make_shared<Render::ModelInstance>();
	//mdlInst->SetMesh("resources/meshes/cube.obj");

	auto mdlInst = ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl");

	gp->setModelInstance(mdlInst);

	this->entity->Activate();
}

void Application::SaveModel(const char* filepath)
{
	if (this->entity != nullptr)
	{
		tinyxml2::XMLDocument data;

		tinyxml2::XMLNode* pRoot = data.NewElement("Vortex");
		data.InsertFirstChild(pRoot);

		Ptr<GraphicsProperty> gp = this->entity->FindProperty<GraphicsProperty>();

		tinyxml2::XMLNode* model = data.NewElement("Model");
		model->ToElement()->SetAttribute("name", gp->getModelInstance()->GetName().AsCharPtr());
		pRoot->InsertFirstChild(model);

		tinyxml2::XMLNode* mesh = data.NewElement("Mesh");
		mesh->ToElement()->SetAttribute("name", gp->getModelInstance()->GetMesh()->GetPath().c_str());
		model->InsertFirstChild(mesh);

		ModelNode* node;
		for (int i = 0; i < gp->getModelInstance()->GetModelNodes().Size(); ++i)
		{
			node = gp->getModelInstance()->GetModelNodes()[i];
			tinyxml2::XMLElement* element = data.NewElement("Node");
			element->SetAttribute("primitivegroup", node->primitiveGroup);
			element->SetAttribute("surface", node->surface->GetPath().AsCharPtr());
			
			mesh->InsertEndChild(element);
		}

		tinyxml2::XMLError eResult = data.SaveFile(filepath);
		assert(!eResult);

		data.Clear();
	}
}

void Application::ExportMesh(const char* filepath)
{
	if (this->entity != nullptr)
	{
		Render::MeshBuilder::ExportMesh(this->entity->FindProperty<GraphicsProperty>()->getModelInstance()->GetMesh(), filepath);
		this->entity->FindProperty<GraphicsProperty>()->getModelInstance()->SetMesh(filepath);
	}
}

} // namespace LevelEditor
