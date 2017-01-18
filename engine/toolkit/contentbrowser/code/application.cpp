//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <fysik/physicsdevice.h>
#include <render/debugrender/debugrenderer.h>
#include "foundation/math/plane.h"
#include "imgui.h"
#include "render/server/frameserver.h"
#include "render/server/lightserver.h"
#include "application/basegamefeature/managers/scenemanager.h"
#include "tinyxml2.h"
#include "render/resources/surface.h"

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
	App::Open();
	this->window = new Display::Window;
	
	keyhandler = BaseGameFeature::KeyHandler::Instance();
	keyhandler->Init(this->window);

	// Initiate everything we need
	// TODO: We should be able to cut down on a lot of this code by creating our own resource structures
	if (this->window->Open())
	{
		//Init RenderDevice
		RenderDevice::Instance()->Initialize();
		// Setup framepasses
		FrameServer::Instance()->SetupFramePasses();
		//Always setup shaders before materials!
		ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
		//Load all materials
		ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");
		//Init debugrenderer. Always do this AFTER setting up shaders!
		Debug::DebugRenderer::Instance()->Initialize();
		//Setup UI
		this->UI = new UserInterface(this);

		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Vortex Content Browser");

		PointLight pLight;
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
		UI->Run();		
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

	while (this->window->IsOpen() && !this->shutdown)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
       	
		Physics::PhysicsDevice::Instance()->Solve();
		BaseGameFeature::EntityManager::Instance()->Update();

		if (ImGui::GetIO().KeyAlt)
		{
			CameraMovement();
		}

		RenderDevice::Instance()->Render(false);

		if (this->UI->selectedNode != nullptr)
		{
			Debug::DebugRenderer::Instance()->DrawMesh(this->loadedModel->GetGraphicsProperty()->getModelInstance()->GetMesh(), this->loadedModel->GetTransform(), Math::vec4(0.5f, 0.5f, 0.0f, 0.5f), Debug::RenderMode::Normal, this->UI->selectedNode->primitiveGroup);
		}

		this->window->SwapBuffers();
	}

	this->window->Close();
}

void Application::NewModel()
{
	if (this->loadedModel != nullptr)
	{
		this->loadedModel->Deactivate();
	}

	this->loadedModel = std::make_shared<Game::ModelEntity>();

	//std::shared_ptr<Render::ModelInstance> mdlInst = std::make_shared<Render::ModelInstance>();
	//mdlInst->SetMesh("resources/meshes/cube.obj");

	auto mdlInst = ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl");

	this->loadedModel->SetModel(mdlInst);

	this->loadedModel->Activate();
}

void Application::SaveModel(const char* filepath)
{
	if (this->loadedModel != nullptr)
	{
		tinyxml2::XMLDocument data;

		tinyxml2::XMLNode* pRoot = data.NewElement("Vortex");
		data.InsertFirstChild(pRoot);

		tinyxml2::XMLNode* model = data.NewElement("Model");
		model->ToElement()->SetAttribute("name", loadedModel->GetGraphicsProperty()->getModelInstance()->GetName().c_str());
		pRoot->InsertFirstChild(model);

		tinyxml2::XMLNode* mesh = data.NewElement("Mesh");
		mesh->ToElement()->SetAttribute("name", loadedModel->GetGraphicsProperty()->getModelInstance()->GetMesh()->GetPath().c_str());
		model->InsertFirstChild(mesh);

		ModelNode* node;
		for (int i = 0; i < loadedModel->GetGraphicsProperty()->getModelInstance()->GetModelNodes().Size(); ++i)
		{
			node = loadedModel->GetGraphicsProperty()->getModelInstance()->GetModelNodes()[i];
			tinyxml2::XMLElement* element = data.NewElement("Node");
			element->SetAttribute("primitivegroup", node->primitiveGroup);
			element->SetAttribute("surface", node->surface->GetPath());
			
			mesh->InsertEndChild(element);
		}

		tinyxml2::XMLError eResult = data.SaveFile(filepath);
		assert(!eResult);

		data.Clear();
	}
}

} // namespace LevelEditor
