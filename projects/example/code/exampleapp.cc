//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <render/server/renderdevice.h>
#include <fysik/physicsserver.h>
#include "foundation/math/plane.h"
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
	this->window->SetSize(1024, 1024);
	this->window->SetTitle("Vortex Engine Test Environment");

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

        physicsCollider = std::make_shared<Physics::SurfaceCollider>();
		physicsCollider1 = std::make_shared<Physics::SurfaceCollider>();

		//Always setup shaders before materials!
		ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
		//Load all materials
		ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");

		//modelInstance->SetMaterial("Static");
		//modelInstance->SetMesh("resources/models/player.nvx2");
		modelInstance->SetMaterial("OBJStatic");
		modelInstance->SetMesh("resources/models/cube.obj");
		gProperty->setModelInstance(modelInstance);

		modelInstance1->SetMaterial("OBJStatic");
		modelInstance1->SetMesh("resources/models/kung.obj");
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

		ImGui::SetWindowSize(ImVec2(450.0f,100.0f), ImGuiSetCond_::ImGuiSetCond_Always);
		// create text editors for shader code
		ImGui::Text("Selected Mesh: %s\n", consoleBuffer.c_str());
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

    Math::mat4 transf = Math::mat4::translation(0.0f, 0.0f, -2.0f);
    Math::mat4 transf2 = Math::mat4::translation(2.0f, 0.0f, 0.0f);

    gProperty->setModelMatrix(transf);
    gProperty1->setModelMatrix(transf2);

    physicsCollider->CookOBJData(modelInstance->GetMesh()->OBJvertexBuffer, modelInstance->GetMesh()->OBJindexBuffer);
	physicsCollider1->CookOBJData(modelInstance1->GetMesh()->OBJvertexBuffer, modelInstance1->GetMesh()->OBJindexBuffer);

    gProperty->setCollider(physicsCollider);
    gProperty1->setCollider(physicsCollider1);

	Physics::PhysicsServer::Instance()->addGraphicsProperty(gProperty);
    Physics::PhysicsServer::Instance()->addGraphicsProperty(gProperty1);

	double cursorPosX = 0.0f;
	double cursorPosY = 0.0f;

    Math::point cameraPos = Math::vec4::zerovector();

    float tempRotation = 0.0f;

    Math::vec4 rayStart = Math::vec4::zerovector();
    Math::vec4 rayEnd= Math::vec4::zerovector();

    Physics::PhysicsHit hit;

	hit.object = nullptr;

	while (this->window->IsOpen())
	{
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

        this->gProperty1->setModelMatrix(Math::mat4::multiply(Math::mat4::rotationyawpitchroll(tempRotation, tempRotation, 0.0f), transf2));

        tempRotation += 0.005f;

        Graphics::MainCamera::Instance()->setViewMatrix(Math::mat4::lookatrh(cameraPos, cameraPos + forward, up));

        Math::mat4 view = Graphics::MainCamera::Instance()->getViewMatrix();
        Math::mat4 invView = Math::mat4::inverse(view);
		Math::mat4 invViewProj = Math::mat4::multiply(invView, invProj);
		Math::mat4 viewProj = Math::mat4::multiply(view, projection);

		if(keyhandler->leftMousePressed)
		{
            printf("\n\n\n\n\n\n\n\n");
			glfwGetCursorPos(this->window->GetGLFWWindow(), &cursorPosX, &cursorPosY);

            printf("CameraPos: %f %f %f %f\n", cameraPos.x(), cameraPos.y(), cameraPos.z(), cameraPos.w());

            // Transform to world coordinates
			cursorPosX = (((cursorPosX / this->window->GetWidth()) -0.5f) * 2.0f);
			cursorPosY = (((cursorPosY / this->window->GetHeight()) - 0.5f) * 2.0f);

            Math::vec4 cursorTransform = Math::vec4(cursorPosX, -cursorPosY, 1.0, 1.0f);

            printf("cursorpos screenspace : %f, %f, %f, %f\n", cursorTransform.x(), cursorTransform.y(), cursorTransform.z(), cursorTransform.w());

			cursorTransform = Math::mat4::transform(cursorTransform, invProj);

			//Pl = Pv * NearPlane * (1,-1,1,1)
            Math::point ray = (cursorTransform * 0.01f);
            //ray.x() = ray.x() * -1.0f;

			Math::vec4 rayWorldPos = Math::mat4::transform(ray, invView);

            printf("rayWorldPos: %f %f %f %f\n", rayWorldPos.x(), rayWorldPos.y(), rayWorldPos.z(), rayWorldPos.w());

            Math::vec4 rayDirection = rayWorldPos - invView.get_position();

            rayDirection = Math::vec4::normalize(rayDirection);

            //Create ray to render
            rayStart = rayWorldPos;

			if(Physics::PhysicsServer::Instance()->Raycast(hit, rayWorldPos, rayDirection, 10.0f))
            {
                printf("--- Hit object! ---\n");
                rayEnd = hit.point;
            }
            else
            {
                rayEnd = rayWorldPos + (rayDirection*10.0f);
            }


		}
		
		RenderDevice::Instance()->Render();

		if(hit.object != nullptr)
			consoleBuffer = hit.object->getModelInstance()->GetMesh()->GetFileName();

		this->gProperty->getbbox().debugRender();
        this->gProperty1->getbbox().debugRender();

		this->gProperty->getCollider()->debugDraw();

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
		


		this->window->SwapBuffers();
	}
}

} // namespace Example
