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

		//Always setup shaders before materials!
		ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
		//Load all materials
		ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");
		
		modelInstance->SetMaterial("Static");
		modelInstance->SetMesh("resources/models/player.nvx2");
		gProperty->setModelInstance(modelInstance);
		
		modelInstance1->SetMaterial("OBJStatic");
		modelInstance1->SetMesh("resources/models/quad_tris.obj");
		gProperty1->setModelInstance(modelInstance1);

		quad = Math::Quad();
		quad.v1 = Math::Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		quad.v2 = Math::Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		quad.v3 = Math::Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		quad.v4 = Math::Vector4(0.5f, -0.5f, 0.0f, 1.0f);

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

		// create text editors for shader code
		ImGui::InputTextMultiline("Vertex Shader", consoleBuffer, CONSOLE_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
								  ImGuiInputTextFlags_AllowTabInput);

		//ImGui::InputTextMultiline("Pixel Shader", fsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
		//						  ImGuiInputTextFlags_AllowTabInput);

		// apply button
		if (ImGui::Button("Apply"))
		{
			// if pressed we compile the shaders
			//	this->CompileShaders();
		}
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

	Math::Matrix4 projection = Graphics::MainCamera::Instance()->getProjectionMatrix();
    Math::Matrix4 invProj = projection.invert();

    Math::Matrix4 transf = Math::Matrix4::translation(0.0f, 0.0f, -2.0f);
    Math::Matrix4 transf2 = Math::Matrix4::translation(2.0f, 0.0f, -1.0f);

    gProperty->setModelMatrix(transf);
    gProperty1->setModelMatrix(transf2);

	//quad.v1 = transf2 * quad.v1;
	//quad.v2 = transf2 * quad.v2;
	//quad.v3 = transf2 * quad.v3;
	//quad.v4 = transf2 * quad.v4;

    Math::Plane plane = Math::Plane(quad.v2, quad.v3, quad.v1);

	double cursorPosX = 0.0f;
	double cursorPosY = 0.0f;

    Math::Vector4 cameraPos = Math::Vector4();

    Util::Array<std::pair<Math::Vector4, Math::Vector4>> rays;

	while (this->window->IsOpen())
	{
		this->window->Update();

        x = 0.0f;
        y = 0.0f;

        if(keyhandler->W)
        {
            y += 0.02f;
        }
        if(keyhandler->S)
        {
            y -= 0.015f;
        }
        if(keyhandler->A)
        {
            x += 0.02f;
        }
        if(keyhandler->D)
        {
            x -= 0.02f;
        }

        Math::Matrix4 roty = Math::Matrix4::rotY(-keyhandler->mouseY);
        Math::Matrix4 rotx = Math::Matrix4::rotX(keyhandler->mouseX);

        Math::Matrix4 rotation = rotx * roty;

        const Math::Vector4& left = rotation.get_xaxis();
        const Math::Vector4& up = rotation.get_yaxis();
        const Math::Vector4& forward = rotation.get_zaxis();

        cameraPos = cameraPos + (left * x);
        cameraPos = cameraPos + (forward * y);

        Graphics::MainCamera::Instance()->setViewMatrix(Graphics::MainCamera::LookAt(cameraPos, cameraPos + forward, up));

        Math::Matrix4 view = Graphics::MainCamera::Instance()->getViewMatrix();
        Math::Matrix4 invView = view.invert();
        Math::Matrix4 invProjView = invProj * invView;

		if(keyhandler->leftMousePressed)
		{
            Math::Plane plane = Math::Plane(projection * view * quad.v1, projection * view * quad.v2, projection * view * quad.v3);

            printf("\n\n\n\n\n\n\n\n");
			glfwGetCursorPos(this->window->GetGLFWWindow(), &cursorPosX, &cursorPosY);

			// Transform to world coordinates
			cursorPosX = ((2.0f * cursorPosX) / this->window->GetWidth()) -1.0f;
			cursorPosY = 1.0f-((2.0f*cursorPosY) / this->window->GetHeight());

            Math::Vector4 cursorTransform = Math::Vector4(cursorPosX, cursorPosY, 1.0, 1.0f);

            printf("cursorpos screenspace : %f, %f, %f, %f\n", cursorTransform.x(), cursorTransform.y(), cursorTransform.z(), cursorTransform.w());

            cursorTransform =  cursorTransform * invProj;

            //Pl = Pv * NearPlane * (1,-1,1,1)
            Math::Vector4 ray = (cursorTransform * 0.01f);
            //ray.x() = ray.x() * -1.0f;

            Math::Vector4 rayWorldPos = ray * invView;

            Math::Vector4 rayDirection = rayWorldPos - invView.getPosition();

            rayDirection = Math::Vector4::normalize(rayDirection);

            //Create ray to render
            std:pair<Math::Vector4, Math::Vector4> r = std::make_pair(rayWorldPos, rayWorldPos + (rayDirection*100.0f));
            rays.Append(r);

            Math::Vector4 hit;

			if(Physics::PhysicsServer::Raycast(hit, rayWorldPos, rayDirection, 100.0f, plane))
            {
                printf("Hit plane at %f, %f, %f, %f\n", hit.x(), hit.y(), hit.z(), hit.w());

                Math::Vector4 modelMid = gProperty1->getModelMatrix().getPosition();

                Math::Vector4 rightEdgeTop = modelMid;
                rightEdgeTop.x() += 0.5f;
                rightEdgeTop.y() += 0.5f;

                //modelMid = projection * modelMid;

                //modelMid = modelMid / modelMid.w();
                //rightEdgeTop = projection * rightEdgeTop;

                //rightEdgeTop = rightEdgeTop / rightEdgeTop.w();

                printf("Model mid: %f, %f, %f, %f\n", modelMid.x(), modelMid.y(), modelMid.z(), modelMid.w());
                printf("Model right top: %f, %f, %f, %f\n", rightEdgeTop.x(), rightEdgeTop.y(), rightEdgeTop.z(), rightEdgeTop.w());
            }


		}

		RenderDevice::Instance()->Render();

        // Render LINES
        glUseProgram(0);
        glEnable(GL_DEPTH_TEST);

        // DRAW QUAD
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);

        Math::Matrix4 t = (projection * view * transf2);

        Math::Vector4 v1 = t * quad.v1;
        Math::Vector4 v2 = t * quad.v2;
        Math::Vector4 v3 = t * quad.v3;

        glVertex3f(v1[0], v1[1], v1[2]);
        glVertex3f(v2[0], v2[1], v2[2]);

        glVertex3f(v2[0], v2[1], v2[2]);
        glVertex3f(v3[0], v3[1], v3[2]);

        glVertex3f(v3[0], v3[1], v3[2]);
        glVertex3f(v1[0], v1[1], v1[2]);
        glEnd();


        //glMatrixMode(GL_MODELVIEW);
        //glLoadMatrixf(view.transpose().get());

        //glMatrixMode(GL_PROJECTION);
        //glLoadMatrixf(projection.transpose().get());

        for (int i = 0; i < rays.Size(); ++i)
        {
            glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f);

            Math::Vector4 v1 = rays[i].first;
            Math::Vector4 v2 = rays[i].second;

            glVertex3f(v1[0], v1[1], v1[2]);
            glVertex3f(v2[0], v2[1], v2[2]);

            glEnd();
        }


		this->window->SwapBuffers();
	}
}

} // namespace Example
