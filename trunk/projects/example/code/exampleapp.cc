//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <fysik/physicsdevice.h>
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
		this->window->SetSize(1600, 900);
		this->window->SetTitle("Vortex Engine Test Environment");

		//Init RenderDevice
		RenderDevice::Instance()->Initialize();
		// Setup framepasses
		FrameServer::Instance()->SetupFramePasses();
		//Always setup shaders before materials!
		ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
		//Load all materials
		ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");
		

		this->consoleBuffer = new char[CONSOLE_BUFFER_SIZE];


		// Load Scene
		modelInstanceScene = std::make_shared<Render::ModelInstance>();
		gPropertyScene = new Render::GraphicsProperty();
		SceneCollider = std::make_shared<Physics::SurfaceCollider>();
		SceneCollider->SetShape(Physics::ColliderShape::BOX);
		SceneEntity = std::make_shared<Game::StaticEntity>();
		SceneEntity->SetCollider(SceneCollider);
		modelInstanceScene->SetMaterial("OBJStatic");
		modelInstanceScene->SetMesh("resources/models/groundfloor.obj");
		gPropertyScene->setModelInstance(modelInstanceScene);
		SceneEntity->SetGraphicsProperty(gPropertyScene);
		SceneCollider->CookOBJData(modelInstanceScene->GetMesh()->OBJvertexBuffer, modelInstanceScene->GetMesh()->OBJindexBuffer, modelInstanceScene->GetMesh()->getBaseBBox());
		SceneEntity->SetTransform(Math::mat4::translation(0.0f, -2.0f, -0.0f));
		SceneEntity->Activate();

		modelInstance = std::make_shared<Render::ModelInstance>();
		modelInstance1 = std::make_shared<Render::ModelInstance>();

		gProperty = new Render::GraphicsProperty();
		gProperty1 = new Render::GraphicsProperty();
		gProperty2 = new Render::GraphicsProperty();
		gProperty3 = new Render::GraphicsProperty();
		gProperty4 = new Render::GraphicsProperty();

        physicsCollider = std::make_shared<Physics::SurfaceCollider>();
		physicsCollider1 = std::make_shared<Physics::SurfaceCollider>();

		physicsCollider->SetShape(Physics::ColliderShape::BOX);
		physicsCollider1->SetShape(Physics::ColliderShape::BOX);

		rBody1 = std::make_shared<Physics::RigidBody>();
        rBody2 = std::make_shared<Physics::RigidBody>();
        rBody3 = std::make_shared<Physics::RigidBody>();
        rBody4 = std::make_shared<Physics::RigidBody>();
		rBody5 = std::make_shared<Physics::RigidBody>();

		rigidBodyEntity1 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity2 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity3 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity4 = std::make_shared<Game::RigidBodyEntity>();
		rigidBodyEntity5 = std::make_shared<Game::RigidBodyEntity>();

        rigidBodyEntity1->SetRigidBody(rBody1);
        rigidBodyEntity2->SetRigidBody(rBody2);
        rigidBodyEntity3->SetRigidBody(rBody3);
        rigidBodyEntity4->SetRigidBody(rBody4);
		rigidBodyEntity5->SetRigidBody(rBody5);

        rigidBodyEntity1->SetCollider(physicsCollider1);
        rigidBodyEntity2->SetCollider(physicsCollider1);
		rigidBodyEntity3->SetCollider(physicsCollider1);
		rigidBodyEntity4->SetCollider(physicsCollider1);
		rigidBodyEntity5->SetCollider(physicsCollider);

		modelInstance->SetMaterial("OBJStatic");
		modelInstance->SetMesh("resources/models/kung.obj");
		gProperty->setModelInstance(modelInstance);

		modelInstance1->SetMaterial("OBJStatic");
		modelInstance1->SetMesh("resources/models/cube.obj");
		gProperty1->setModelInstance(modelInstance1);
		gProperty2->setModelInstance(modelInstance1);
		gProperty3->setModelInstance(modelInstance1);
		gProperty4->setModelInstance(modelInstance1);

		PointLight pLight;
		pLight.position = Math::vec4(3.0f, 2.0f, 1.0f, 1.0f);
		pLight.color = Math::vec4(0.5f, 0.5f, 0.5f, 1.0f);
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

        rigidBodyEntity1->SetGraphicsProperty(gProperty1);
        rigidBodyEntity2->SetGraphicsProperty(gProperty2);
        rigidBodyEntity3->SetGraphicsProperty(gProperty3);
        rigidBodyEntity4->SetGraphicsProperty(gProperty4);
        rigidBodyEntity5->SetGraphicsProperty(gProperty);

        physicsCollider->CookOBJData(modelInstance->GetMesh()->OBJvertexBuffer, modelInstance->GetMesh()->OBJindexBuffer, modelInstance->GetMesh()->getBaseBBox());
        physicsCollider1->CookOBJData(modelInstance1->GetMesh()->OBJvertexBuffer, modelInstance1->GetMesh()->OBJindexBuffer, modelInstance1->GetMesh()->getBaseBBox());

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

		ImGui::SetWindowSize(ImVec2(450.0f,210.0f), ImGuiSetCond_::ImGuiSetCond_Once);

		ImGui::Text("Frame time: %f\n", this->frameTime);

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

	Math::mat4 projection = Graphics::MainCamera::Instance()->getProjectionMatrix();
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
        if(keyhandler->W)
        {
            translation.z() += 0.02f;
        }
        if(keyhandler->S)
        {
            translation.z() -= 0.015f;
        }
        if(keyhandler->A)
        {
            translation.x() -= 0.02f;
        }
        if(keyhandler->D)
        {
            translation.x() += 0.02f;
        }

		Math::mat4 xMat = Math::mat4::rotationx(-keyhandler->mouseX * 0.01f);
		Math::mat4 yMat = Math::mat4::rotationy(-keyhandler->mouseY * 0.01f);
		Math::mat4 rotation = Math::mat4::multiply(xMat, yMat);

		//Math::mat4 rotation = Math::mat4::rotationyawpitchroll(nvgDegToRad(keyhandler->mouseY) * 0.5f, nvgDegToRad(keyhandler->mouseX) * 0.5f, 0.0f);

		const Math::point& left = rotation.get_xaxis();
		const Math::point& up = rotation.get_yaxis();
        const Math::point& forward = rotation.get_zaxis();

		translation = Math::mat4::transform(translation, rotation);
		cameraPos += translation;

		Graphics::MainCamera::Instance()->SetPosition(cameraPos);
        Graphics::MainCamera::Instance()->LookAt(cameraPos + forward, up);

        Math::mat4 view = Graphics::MainCamera::Instance()->getViewMatrix();
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

            Math::vec4 cursorTransform = Math::vec4(cursorPosX, -cursorPosY, 1.0, 1.0f);

            printf("cursorpos screenspace : %f, %f, %f, %f\n", cursorTransform.x(), cursorTransform.y(), cursorTransform.z(), cursorTransform.w());

			cursorTransform = Math::mat4::transform(cursorTransform, invProj);

            Math::point ray = (cursorTransform * 0.01f);

			Math::vec4 rayWorldPos = Math::mat4::transform(ray, invView);

            printf("rayWorldPos: %f %f %f %f\n", rayWorldPos.x(), rayWorldPos.y(), rayWorldPos.z(), rayWorldPos.w());

            Math::vec4 rayDirection = rayWorldPos - invView.get_position();

            rayDirection = Math::vec4::normalize(rayDirection);

            //Create ray to render
            rayStart = rayWorldPos;

			if(Physics::PhysicsServer::Instance()->Raycast(hit, rayWorldPos, rayDirection, 10.0f))
            {
                printf("--- Hit object! ---\n");
                Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(hit.object);

                if(rbe != nullptr)
                    rbe->GetRigidBody()->applyForceAtPoint(rayDirection, .1f, hit.point);

                rayEnd = hit.point;

				PointLight pLight;
				pLight.position = hit.point + (hit.surfaceNormal);
				pLight.color = Math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
				pLight.radiusAndPadding.set_x(10.0f);

				LightServer::Instance()->AddPointLight(pLight);
			}
            else
            {
                rayEnd = rayWorldPos + (rayDirection*10.0f);
            }

			double endtime = glfwGetTime();
			printf("Raycast time elapsed: %f\n", endtime - time);
		}
		
		Physics::PhysicsDevice::Instance()->Solve();

        this->rigidBodyEntity1->Update();
        this->rigidBodyEntity2->Update();
		this->rigidBodyEntity3->Update();
		this->rigidBodyEntity4->Update();
		this->rigidBodyEntity5->Update();

		RenderDevice::Instance()->Render();

		if(hit.object != nullptr)
        {
            Game::PhysicsEntity* pe = dynamic_cast<Game::PhysicsEntity*>(hit.object);

            if(pe != nullptr)
                consoleBuffer = pe->GetGraphicsProperty()->getModelInstance()->GetMesh()->GetFileName();

        }

		/*
		this->gProperty->getbbox().debugRender();
        this->gProperty1->getbbox().debugRender();

		//this->gProperty->getCollider()->debugDraw();

		
        // Render LINES
        glUseProgram(0);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((GLfloat*)&view.mat.m[0][0]);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((GLfloat*)&projection.mat.m[0][0]);

        glBegin(GL_LINES);

        Math::point v1 = rayStart;
        Math::point v2 = rayEnd;

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex4f(v1[0], v1[1], v1[2], v1[3]);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex4f(v2[0], v2[1], v2[2], v2[3]);

        Math::point hitp = hit.point;
        Math::point hitn = hit.point + (hit.surfaceNormal * 0.1f);

        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex4f(hitp[0], hitp[1], hitp[2], hitp[3]);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex4f(hitn[0], hitn[1], hitn[2], hitn[3]);

        glEnd();
		*/



		this->window->SwapBuffers();

		frameTime = glfwGetTime() - time;

	}
}

} // namespace Example
