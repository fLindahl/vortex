//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <fysik/physicsdevice.h>
#include <render/debugrender/debugrenderer.h>
#include "foundation/math/plane.h"
#include "imgui.h"
#include "render/server/frameserver.h"
#include "render/server/lightserver.h"

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


		//Never set resolution before initializing rendering and framepasses
		this->window->SetSize(1600, 900);
		this->window->SetTitle("Vortex Engine Test Environment");

		//Set render resolution. This is set with window->setsize too so we override it here
		RenderDevice::Instance()->SetRenderResolution(1600, 900);

		this->consoleBuffer = new char[CONSOLE_BUFFER_SIZE];
		
		// Load Scene
		modelInstanceScene = ResourceServer::Instance()->LoadModel("resources/models/groundfloor.mdl");

		SceneEntity1 = std::make_shared<Game::StaticEntity>();
		SceneEntity2 = std::make_shared<Game::StaticEntity>();
		SceneEntity3 = std::make_shared<Game::StaticEntity>();
		SceneEntity4 = std::make_shared<Game::StaticEntity>();
		SceneEntity5 = std::make_shared<Game::StaticEntity>();
		SceneEntity6 = std::make_shared<Game::StaticEntity>();

		SceneEntity1->SetModel(modelInstanceScene);
		SceneEntity2->SetModel(modelInstanceScene);
		SceneEntity3->SetModel(modelInstanceScene);
		SceneEntity4->SetModel(modelInstanceScene);
		SceneEntity5->SetModel(modelInstanceScene);
		SceneEntity6->SetModel(modelInstanceScene);

		SceneEntity1->SetTransform(Math::mat4::translation(0.0f, -2.0f, 0.0f));
		SceneEntity2->SetTransform(Math::mat4::multiply(Math::mat4::rotationz(1.57f), Math::mat4::translation(10.0f, 8.0f, 0.0f)));
		SceneEntity3->SetTransform(Math::mat4::multiply(Math::mat4::rotationz(1.57f), Math::mat4::translation(-10.0f, 8.0f, 0.0f)));
		SceneEntity4->SetTransform(Math::mat4::multiply(Math::mat4::rotationx(1.57f), Math::mat4::translation(0.0f, 8.0f, 10.0f)));
		SceneEntity5->SetTransform(Math::mat4::multiply(Math::mat4::rotationx(1.57f), Math::mat4::translation(0.0f, 8.0f, -10.0f)));
		SceneEntity6->SetTransform(Math::mat4::translation(0.0f, 18.0f, 0.0f));

		SceneEntity1->Activate();
		SceneEntity2->Activate();
		SceneEntity3->Activate();
		SceneEntity4->Activate();
		SceneEntity5->Activate();
		SceneEntity6->Activate();

		modelInstance = ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl");

		rigidBodyEntity1 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity2 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity3 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity4 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity5 = std::make_shared<Game::RigidBodyEntity>();

		rigidBodyEntity1->SetModel(modelInstance);
		rigidBodyEntity2->SetModel(modelInstance);
		rigidBodyEntity3->SetModel(modelInstance);
		rigidBodyEntity4->SetModel(modelInstance);
		rigidBodyEntity5->SetModel(modelInstance);

		Math::mat4 transf1 = Math::mat4::translation(2.0f, -0.5f, 1.5f);
		Math::mat4 transf2 = Math::mat4::translation(0.0f, -0.5f, 1.5f);
		Math::mat4 transf3 = Math::mat4::translation(2.0f, -0.5f, -1.5f);
		Math::mat4 transf4 = Math::mat4::translation(0.0f, -0.5f, -1.5f);
		Math::mat4 transf5 = Math::mat4::translation(0.0f, -0.5f, -3.0f);

		this->rigidBodyEntity1->SetTransform(transf1);
		this->rigidBodyEntity2->SetTransform(transf2);
		this->rigidBodyEntity3->SetTransform(transf3);
		this->rigidBodyEntity4->SetTransform(transf4);
		this->rigidBodyEntity5->SetTransform(transf5);

		rigidBodyEntity1->Activate();
		rigidBodyEntity2->Activate();
		rigidBodyEntity3->Activate();
		rigidBodyEntity4->Activate();
		rigidBodyEntity5->Activate();

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

        const int numEntsX = 2;
        const int numEntsY = 2;

        for (int i = 0; i < numEntsX; ++i) {
            for (int j = 0; j < numEntsY; ++j) {
                std::shared_ptr<Game::RigidBodyEntity> ent = std::make_shared<Game::RigidBodyEntity>();
                RBEs.Append(ent);
				ent->SetModel(modelInstance);

                Math::mat4 transf1 = Math::mat4::translation(i*1.0f - (numEntsX/2), numEntsY/4 + j*1.0f, 5.0f);
                ent->SetTransform(transf1);
                ent->Activate();
            }
        }

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

void ShowExampleMenuFile()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors"))
	{
		for (int i = 0; i < ImGuiCol_COUNT; i++)
			ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}


void ExampleApp::RenderUI()
{
	if (this->window->IsOpen())
	{
		bool show = true;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// create a new window
		ImGui::Begin("Console", &show, ImGuiWindowFlags_NoSavedSettings);

		ImGui::SetWindowSize(ImVec2(450.0f,210.0f), ImGuiSetCond_::ImGuiSetCond_Once);

		ImGui::Text("Frame time: %f\n", this->frameTime);

		currentFPS = 1.0f / this->frameTime;
		
		ImGui::Text("FPS: %f\n", currentFPS);

		// create text editors for shader code
		ImGui::Text("Selected Mesh: %s\n", consoleBuffer.c_str());

        if(Physics::PhysicsDevice::Instance()->hasCollision)
            a = "#########################################\n#########################################\n#########################################\n#########################################\n#########################################\n#########################################\n#########################################\n#########################################\n#########################################\n";
        else
            a = "0";

        ImGui::Text("Collision: %s\n", a.c_str());



		if (hit.object != nullptr)
		{
			//ImGui::Text("Orientation: %f, %f, %f, %f\n", std::static_pointer_cast<Game::PhysicsEntity>(hit.object)->GetRigidBody->getOrientation().x(), hit.object->rigidBody->getOrientation().y(), hit.object->rigidBody->getOrientation().z(), hit.object->rigidBody->getOrientation().w());
			//ImGui::Text("Position: %f, %f, %f, %f\n", hit.object->rigidBody->getPosition().x(), hit.object->rigidBody->getPosition().y(), hit.object->rigidBody->getPosition().z(), hit.object->rigidBody->getPosition().w());
			//ImGui::Text("LinearVelocity: %f, %f, %f, %f\n", hit.object->rigidBody->getLinearVelocity().x(), hit.object->rigidBody->getLinearVelocity().y(), hit.object->rigidBody->getLinearVelocity().z(), hit.object->rigidBody->getLinearVelocity().w());
			//ImGui::Text("AngularVelocity: %f, %f, %f, %f\n", hit.object->rigidBody->getAngularVelocity().x(), hit.object->rigidBody->getAngularVelocity().y(), hit.object->rigidBody->getAngularVelocity().z(), hit.object->rigidBody->getAngularVelocity().w());
			//ImGui::Text("Acceleration: %f, %f, %f, %f\n", hit.object->rigidBody->getAcceleration().x(), hit.object->rigidBody->getAcceleration().y(), hit.object->rigidBody->getAcceleration().z(), hit.object->rigidBody->getAcceleration().w());
		}
		
		//ImGui::InputTextMultiline("Vertex Shader", consoleBuffer, CONSOLE_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);

		//ImGui::InputTextMultiline("Pixel Shader", fsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
		//						  ImGuiInputTextFlags_AllowTabInput);

		// apply button
		//if (ImGui::Button("Apply"))
		//{
			// if pressed we compile the shaders
			//	this->CompileShaders();
		//}
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

	Math::mat4 projection = Graphics::MainCamera::Instance()->getProjection();
    Math::mat4 invProj = Math::mat4::inverse(projection);

	double cursorPosX = 0.0f;
	double cursorPosY = 0.0f;

    Math::point cameraPos = Math::vec4::zerovector();

    Math::vec4 rayStart = Math::vec4::zerovector();
    Math::vec4 rayEnd= Math::vec4::zerovector();

	hit.object = nullptr;

	while (this->window->IsOpen())
	{
		double time = glfwGetTime();
		this->window->Update();
		
		Math::vec4 translation = Math::vec4::zerovector();

        const float speedIncrease = 0.02f;
        float speedMultiplier = 1.0f;

        if(keyhandler->leftShift)
        {
            speedMultiplier = 3.0f;
        }
        if(keyhandler->W)
        {
                translation.z() += speedIncrease * speedMultiplier;
        }
        if(keyhandler->S)
        {
            translation.z() -= speedIncrease * speedMultiplier;
        }
        if(keyhandler->A)
        {
            translation.x() -= speedIncrease * speedMultiplier;
        }
        if(keyhandler->D)
        {
            translation.x() += speedIncrease * speedMultiplier;
        }

		Math::mat4 xMat = Math::mat4::rotationx((float)-keyhandler->mouseX * 0.01f);
		Math::mat4 yMat = Math::mat4::rotationy((float)-keyhandler->mouseY * 0.01f);
		Math::mat4 rotation = Math::mat4::multiply(xMat, yMat);

		//Math::mat4 rotation = Math::mat4::rotationyawpitchroll(nvgDegToRad(keyhandler->mouseY) * 0.5f, nvgDegToRad(keyhandler->mouseX) * 0.5f, 0.0f);

		const Math::point& left = rotation.get_xaxis();
		const Math::point& up = rotation.get_yaxis();
        const Math::point& forward = rotation.get_zaxis();

		translation = Math::mat4::transform(translation, rotation);
		cameraPos += translation;

		Graphics::MainCamera::Instance()->SetPosition(cameraPos);
        Graphics::MainCamera::Instance()->LookAt(cameraPos + forward, up);

        Math::mat4 view = Graphics::MainCamera::Instance()->getView();
        Math::mat4 invView = Math::mat4::inverse(view);
		Math::mat4 invViewProj = Math::mat4::multiply(invView, invProj);
		Math::mat4 viewProj = Math::mat4::multiply(view, projection);

		if(keyhandler->leftMousePressed)
		{
            printf("\n\n\n\n\n\n\n\n");
			double time = glfwGetTime();
			glfwGetCursorPos(this->window->GetGLFWWindow(), &cursorPosX, &cursorPosY);

            printf("CameraPos: %f %f %f %f\n", cameraPos.x(), cameraPos.y(), cameraPos.z(), cameraPos.w());

            // Transform to world coordinates
			cursorPosX = (((cursorPosX / this->window->GetWidth()) -0.5f) * 2.0f);
			cursorPosY = (((cursorPosY / this->window->GetHeight()) - 0.5f) * 2.0f);

			Math::vec4 cursorTransform = Math::vec4((float)cursorPosX, (float)-cursorPosY, 1.0, 1.0f);

            printf("cursorpos screenspace : %f, %f, %f, %f\n", cursorTransform.x(), cursorTransform.y(), cursorTransform.z(), cursorTransform.w());

			cursorTransform = Math::mat4::transform(cursorTransform, invProj);

            Math::point ray = (cursorTransform * 0.01f);

			Math::vec4 rayWorldPos = Math::mat4::transform(ray, invView);

            printf("rayWorldPos: %f %f %f %f\n", rayWorldPos.x(), rayWorldPos.y(), rayWorldPos.z(), rayWorldPos.w());

            Math::vec4 rayDirection = rayWorldPos - invView.get_position();

            rayDirection = Math::vec4::normalize(rayDirection);

            //Create ray to render
            rayStart = rayWorldPos;

			if(Physics::PhysicsServer::Instance()->Raycast(hit, rayWorldPos, rayDirection, 40.0f))
            {
                printf("--- Hit object! ---\n");
                Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(hit.object);

                if(rbe != nullptr)
                    rbe->GetRigidBody()->applyForceAtPoint(rayDirection, .1f, hit.point);

                rayEnd = hit.point;

                if(keyhandler->rightMousePressed)
                {
                    PointLight pLight;
                    pLight.position = hit.point + (hit.surfaceNormal);
                    pLight.color = Math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
                    pLight.radiusAndPadding.set_x(5.0f);

                    LightServer::Instance()->AddPointLight(pLight);
                }
			}
            else
            {
                rayEnd = rayWorldPos + (rayDirection*10.0f);
            }

			double endtime = glfwGetTime();
			printf("Raycast time elapsed: %f\n", endtime - time);
		}
		
		Physics::PhysicsDevice::Instance()->Solve();

        BaseGameFeature::EntityManager::Instance()->Update();

		Debug::DebugRenderer::Instance()->DrawLine(rayStart, rayEnd, 2.0f, Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		RenderDevice::Instance()->Render();

		if(hit.object != nullptr)
        {
            Game::PhysicsEntity* pe = dynamic_cast<Game::PhysicsEntity*>(hit.object);

			if (pe != nullptr)
			{
				consoleBuffer = pe->GetGraphicsProperty()->getModelInstance()->GetMesh()->GetName();
				Debug::DebugRenderer::Instance()->DrawMesh(pe->GetGraphicsProperty()->getModelInstance()->GetMesh(), pe->GetTransform(), Math::vec4(1.0f, 1.0f, 1.0f, 1.0f), Debug::RenderMode::WireFrame, -1, 2.0f);
			}
        }

		this->window->SwapBuffers();

		this->frameTime = glfwGetTime() - time;

	}
}

} // namespace Example
