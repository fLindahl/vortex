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

#include "application/basegamefeature/keyhandler.h"
#include "render/server/shadowserver.h"
#include "application/basegamefeature/managers/envmanager.h"

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
	this->commandManager = Edit::CommandManager::Instance();
	cameraPos = Math::point::zerovector();
	camRotX = 0;
	camRotY = 0;

	this->rayStart = Math::vec4::zerovector();
	this->rayEnd = Math::vec4::zerovector();
	this->reflectStart = Math::vec4::zerovector();
	this->reflectEnd = Math::vec4::zerovector();
	hit.object = nullptr;

	this->pickingPixels = new GLuint[1920 * 1020];

	this->renderGeoProxies = false;
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

	//Always call app::open _AFTER_ initializing a glfwwindow
	if (this->window->Open() && App::Open())
	{
		keyhandler = BaseGameFeature::KeyHandler::Instance();
		keyhandler->Init(this->window);
		//Setup UI
		this->UI = new UserInterface(this);
		
		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1920, 1020);
		this->window->SetTitle("Vortex Level Editor");

		//RenderDevice::Instance()->SetRenderResolution(256, 256);

		//this->rayStart = Math::vec4::zerovector();
		//this->rayEnd = Math::vec4::zerovector();
		
		//Load Sponza
		this->sponza = std::make_shared<Game::ModelEntity>();
		this->sponza->SetModel(ResourceServer::Instance()->LoadModel("resources/models/sponza.mdl"));
		this->sponza->Activate();
		Math::mat4 sTransform = Math::mat4::scaling(0.01f, 0.01f, 0.01f);
		sTransform.translate(Math::vector(0.0f, -2.0f, 0.0f));
		this->sponza->SetTransform(sTransform);

		//Spawns a cube
		this->wall1 = std::make_shared<Game::ModelEntity>();
		this->wall1->SetModel(ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));
		this->wall1->Activate();
		this->wall1->SetTransform(Math::mat4::translation(-0.063, -1.0f, -0.27f));

		this->wall2 = std::make_shared<Game::ModelEntity>();
		this->wall2->SetModel(ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));
		this->wall2->Activate();
		this->wall2->SetTransform(Math::mat4::translation(-4.19, -1.0f, -0.27f));

		/*
		this->wall1 = std::make_shared<Game::ModelEntity>();
		this->wall2 = std::make_shared<Game::ModelEntity>();
		this->wall3 = std::make_shared<Game::ModelEntity>();
		this->wall4 = std::make_shared<Game::ModelEntity>();
		this->floor = std::make_shared<Game::ModelEntity>();
		this->ceiling = std::make_shared<Game::ModelEntity>();

		this->wall1->SetModel(ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl"));
		this->wall2->SetModel(ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl"));
		this->wall3->SetModel(ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl"));
		this->wall4->SetModel(ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl"));
		this->floor->SetModel(ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl"));
		this->ceiling->SetModel(ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl"));

		this->wall1->Activate();
		this->wall2->Activate();
		this->wall3->Activate();
		this->wall4->Activate();
		this->floor->Activate();
		this->ceiling->Activate();

		this->floor->SetTransform(Math::mat4::translation(0.0f, -2.0f, 0.0f));
		this->wall2->SetTransform(Math::mat4::multiply(Math::mat4::rotationz(1.57f), Math::mat4::translation(10.0f, 8.0f, 0.0f)));
		this->wall3->SetTransform(Math::mat4::multiply(Math::mat4::rotationz(1.57f), Math::mat4::translation(-10.0f, 8.0f, 0.0f)));
		this->wall4->SetTransform(Math::mat4::multiply(Math::mat4::rotationx(1.57f), Math::mat4::translation(0.0f, 8.0f, 10.0f)));
		this->wall1->SetTransform(Math::mat4::multiply(Math::mat4::rotationx(1.57f), Math::mat4::translation(0.0f, 8.0f, -10.0f)));
		this->ceiling->SetTransform(Math::mat4::translation(0.0f, 18.0f, 0.0f));
		*/


/*		PointLight pLight;
		pLight.position = Math::vec4(-3.0f, 0.0f, -2.5f, 1.0f);
		pLight.color = Math::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		pLight.radiusAndPadding.set_x(5.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(-6.0f, 0.0f, 2.5f, 1.0f);
		pLight.color = Math::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(2.0f, -1.0f, -0.0f, 1.0f);
		pLight.color = Math::vec4(0.3f, 0.5f, 0.7f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);

		pLight.position = Math::vec4(0.0f, -1.5f, 0.0f, 1.0f);
		pLight.color = Math::vec4(0.1f, 0.5f, 0.1f, 1.0f);
		LightServer::Instance()->AddPointLight(pLight);*/
/*
     	Render::LightServer::SpotLight sLight;
	    sLight.position = Math::vec4(0.0f, -.5f, -1.0f, 1.0f);
		sLight.color = Math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		sLight.coneDirection = Math::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
		sLight.length = 10.0f;
        sLight.angle = 30.0f;
		LightServer::Instance()->AddSpotLight(sLight);*/

		/*sLight.position = Math::vec4(4.0f, 3.5f, 0.0f, 1.0f);
        sLight.color = Math::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        sLight.coneDirection = Math::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        sLight.length = 15.0f;
        sLight.angle = 25.0f;
        LightServer::Instance()->AddSpotLight(sLight);

         sLight.position = Math::vec4(-2.1f, 0.0f, 3.0f, 1.0f);
        sLight.color = Math::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        sLight.coneDirection = Math::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        sLight.length = 15.0f;
        LightServer::Instance()->AddSpotLight(sLight);

        sLight.position = Math::vec4(-2.1f, 15.0f, -6.0f, 1.0f);
        sLight.color = Math::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        sLight.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
        sLight.length = 18.0f;
        sLight.angle = 5.0f;
        LightServer::Instance()->AddSpotLight(sLight);*/

		/// Creates a directional light in the scene
		//Render::LightServer::Instance()->CreateDirectionalLight(Math::vec4(0.0f, 0.0f, 0.0f, 1.0f), Math::vec4(1.0f, 0.8f, 0.6f, 0.0f));

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
    float a = 0.0f;
	while (this->window->IsOpen() && !this->shutdown)
	{
		double time = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
       	
		Physics::PhysicsDevice::Instance()->Solve();
		BaseGameFeature::EntityManager::Instance()->Update();

		if (ImGui::GetIO().KeyAlt)
		{
			CameraMovement();
		}

		//Debug::DebugRenderer::Instance()->DrawLine(this->rayStart, this->rayEnd, 4.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//Debug::DebugRenderer::Instance()->DrawLine(this->reflectStart, this->reflectEnd, 4.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		//if(Render::LightServer::Instance()->GetNumSpotLights() != 0)
		//	Debug::DebugRenderer::Instance()->DrawLine(	Render::LightServer::Instance()->GetSpotLightAtIndex(0).position, 
		//												Render::LightServer::Instance()->GetSpotLightAtIndex(0).position + Render::LightServer::Instance()->GetSpotLightAtIndex(0).coneDirection * Render::LightServer::Instance()->GetSpotLightAtIndex(0).length,
		//												4.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		//Render::ShadowServer::Instance()->GetSpotLightEntities()[0]->GetSpotLightDirection();

        //a += 0.0001f;
        //Render::LightServer::Instance()->GetSpotLightAtIndex(0).position = Math::vec4(Render::LightServer::Instance()->GetSpotLightAtIndex(0).position.x() + a, 2.3f, 3.0f, 1.0f);
        //Render::LightServer::Instance()->Update();

		if (renderGeoProxies)
		{
			BaseGameFeature::EnvManager::Instance()->RenderGeometryProxies();
		}

		if (this->hit.object != nullptr)
		{
			Game::PhysicsEntity* e = dynamic_cast<Game::PhysicsEntity*>(hit.object);
			if (e != nullptr)
			{
				Debug::DebugRenderer::Instance()->DrawMesh(e->GetGraphicsProperty()->getModelInstance()->GetMesh(), e->GetTransform(), Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), Debug::RenderMode::WireFrame, -1, 2.0f);
			}
		}

		RenderDevice::Instance()->Render(false);

		this->window->SwapBuffers();
		UI->frameTime = glfwGetTime() - time;
	}

	this->window->Close();
}

} // namespace LevelEditor
