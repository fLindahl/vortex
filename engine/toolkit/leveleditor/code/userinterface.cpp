#include "userinterface.h"
#include "render/server/resourceserver.h"
#include "imgui.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "toolkit/tools/style.h"

#include "basetool.h"
#include "selecttool.h"
#include "translatetool.h"

#define CONSOLE_BUFFER_SIZE 8096

namespace Toolkit
{
	UserInterface::UserInterface(LevelEditor::Application* app)
	{
		this->application = app;

		this->selectTool = new Tools::SelectTool();
		this->translateTool = new Tools::TranslateTool();
		this->currentTool = this->selectTool;

		//Load textures
		this->selectToolTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/cursor.png")->GetHandle();
		this->translateToolTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/translate.png")->GetHandle();
		this->rotateToolTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/rotate_stylized.png")->GetHandle();
		this->scaleToolTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/scale_stylized.png")->GetHandle();
		this->entityToolTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/entity.png")->GetHandle();
		this->brushToolTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/cube.png")->GetHandle();
		this->polygonEditTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/polygon.png")->GetHandle();

		this->commandManager = Edit::CommandManager::Instance();

		//Setup ImGui Stuff
		SetupImGuiStyle();
		ImGui::LoadDock("engine/toolkit/leveleditor/layout/default.layout");
	}

	UserInterface::~UserInterface()
	{
		delete this->selectTool;
		delete this->translateTool;
	}

	void UserInterface::Run()
	{
		static bool showStatistics = false;

		RenderDocks();

		//TODO: Make sure we're not editing a textbox before querying for shortcuts
		ExecShortCuts();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ShowFileMenu();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) { commandManager->Undo(); }
				if (ImGui::MenuItem("Redo", "CTRL+Y")) { commandManager->Redo(); }  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::BeginMenu("Show"))
				{
					if (ImGui::MenuItem("Statistics", NULL, &showStatistics)) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// create a new window
		if (showStatistics)
		{
			ImGui::Begin("Statistics", &showStatistics, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ShowBorders);

			ImGui::SetWindowSize(ImVec2(450.0f, 210.0f), ImGuiSetCond_::ImGuiSetCond_Once);
			ImGui::Text("Frame time: %f\n", this->frameTime);

			currentFPS = 1.0f / this->frameTime;

			ImGui::Text("FPS: %f\n", currentFPS);

			ImGui::End();
		}
	}

	void UserInterface::ShowFileMenu()
	{
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("example1.map");
			ImGui::MenuItem("example2.map");
			ImGui::MenuItem("example3.map");
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}
		ImGui::Separator();

		if (ImGui::BeginMenu("Layout"))
		{
			if (ImGui::MenuItem("Save Layout...")) { ImGui::SaveDock("engine/toolkit/leveleditor/layout/default.layout"); }
			if (ImGui::MenuItem("Load Layout...")) { ImGui::LoadDock("engine/toolkit/leveleditor/layout/default.layout"); }
			ImGui::EndMenu();
		}

		//if (ImGui::BeginMenu("Options"))
		//{
		//	ImGui::EndMenu();
		//}
		//if (ImGui::BeginMenu("Colors"))
		//{
		//for (int i = 0; i < ImGuiCol_COUNT; i++)
		//	ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
		//ImGui::EndMenu();
		//}
		//if (ImGui::BeginMenu("Disabled", false)) // Disabled
		//{
		//	IM_ASSERT(0);
		//}
		//if (ImGui::MenuItem("Checked", NULL, true)) {}
		if (ImGui::MenuItem("Quit", "Alt+F4")) { application->shutdown = true; }
	}

	void UserInterface::ExecShortCuts()
	{
		if ((ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Y))) ||
			(ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Z))))
		{
			commandManager->Redo();
		}
		else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Z)))
		{
			commandManager->Undo();
		}
	}

	void UserInterface::RenderDocks()
	{
		const int toolbarWidth = 52;
		const int toolButtonSize = 32;

		ImGui::Begin("ToolBar", NULL,
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoBringToFrontOnFocus);
		{
			ImGui::SetWindowSize(ImVec2(toolbarWidth, application->window->GetHeight() - 16), ImGuiSetCond_Once);
			ImGui::SetWindowPos(ImVec2(0, 16), ImGuiSetCond_Once);

			if (ImGui::ImageButton((void*)this->selectToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				this->currentTool = selectTool;
			}
			if (ImGui::ImageButton((void*)this->translateToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				this->currentTool = translateTool;
			}
			if (ImGui::ImageButton((void*)this->rotateToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = rotateTool;
			}
			if (ImGui::ImageButton((void*)this->scaleToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = scaleTool;
			}
			if (ImGui::ImageButton((void*)this->entityToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = entityTool;
			}
			if (ImGui::ImageButton((void*)this->brushToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = brushTool;
			}
			if (ImGui::ImageButton((void*)this->polygonEditTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = polygonEditTool;
			}

			ImGui::End();
		}
		
		ImGui::RootDock(ImVec2(toolbarWidth, 16), ImVec2(application->window->GetWidth() - toolbarWidth, application->window->GetHeight() - 16));
		{
			ImGui::BeginDock("3D View", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				ImVec2 dockSize = ImGui::GetWindowSize();
				ImGui::Image((void*)Render::FrameServer::Instance()->GetFinalColorBuffer(), dockSize);

				if (ImGui::IsItemHovered())
				{
					application->DoPicking();
				}
			}
			ImGui::EndDock();

			//ImGui::BeginDock("Top View", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |	ImGuiWindowFlags_NoScrollWithMouse);
			//ImGui::EndDock();

			//ImGui::BeginDock("Right View", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |	ImGuiWindowFlags_NoScrollWithMouse);
			//ImGui::EndDock();

			//ImGui::BeginDock("Front View", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |	ImGuiWindowFlags_NoScrollWithMouse);
			//ImGui::EndDock();

			ImGui::BeginDock("Inspector", NULL, ImGuiWindowFlags_NoSavedSettings);
			{
				if (application->hit.object != nullptr)
				{
					ImGui::Text("Transform:");
					ImGui::Text("%f | %f | %f | %f", application->hit.object->GetTransform().getrow(0).x(), application->hit.object->GetTransform().getrow(0).y(), application->hit.object->GetTransform().getrow(0).z(), application->hit.object->GetTransform().getrow(0).w());
					ImGui::Text("%f | %f | %f | %f", application->hit.object->GetTransform().getrow(1).x(), application->hit.object->GetTransform().getrow(1).y(), application->hit.object->GetTransform().getrow(1).z(), application->hit.object->GetTransform().getrow(1).w());
					ImGui::Text("%f | %f | %f | %f", application->hit.object->GetTransform().getrow(2).x(), application->hit.object->GetTransform().getrow(2).y(), application->hit.object->GetTransform().getrow(2).z(), application->hit.object->GetTransform().getrow(2).w());
					ImGui::Text("%f | %f | %f | %f", application->hit.object->GetTransform().getrow(3).x(), application->hit.object->GetTransform().getrow(3).y(), application->hit.object->GetTransform().getrow(3).z(), application->hit.object->GetTransform().getrow(3).w());

					Game::PhysicsEntity* pe = dynamic_cast<Game::PhysicsEntity*>(application->hit.object);

					if (pe != nullptr)
					{
						ImGui::Text("Mesh: %s", pe->GetGraphicsProperty()->getModelInstance()->GetMesh()->GetName().c_str());
						ImGui::Text("Type: %i", pe->GetPhysicsType());

						if (pe->GetPhysicsType() == Physics::PhysicsType::Rigidbody)
						{
							Game::RigidBodyEntity* rb = dynamic_cast<Game::RigidBodyEntity*>(pe);

							ImGui::Text("Orientation: %f, %f, %f, %f\n", rb->GetRigidBody()->getOrientation().x(), rb->GetRigidBody()->getOrientation().y(), rb->GetRigidBody()->getOrientation().z(), rb->GetRigidBody()->getOrientation().w());
							ImGui::Text("Position: %f, %f, %f, %f\n", rb->GetRigidBody()->getPosition().x(), rb->GetRigidBody()->getPosition().y(), rb->GetRigidBody()->getPosition().z(), rb->GetRigidBody()->getPosition().w());
							ImGui::Text("LinearVelocity: %f, %f, %f, %f\n", rb->GetRigidBody()->getLinearVelocity().x(), rb->GetRigidBody()->getLinearVelocity().y(), rb->GetRigidBody()->getLinearVelocity().z(), rb->GetRigidBody()->getLinearVelocity().w());
							ImGui::Text("AngularVelocity: %f, %f, %f, %f\n", rb->GetRigidBody()->getAngularVelocity().x(), rb->GetRigidBody()->getAngularVelocity().y(), rb->GetRigidBody()->getAngularVelocity().z(), rb->GetRigidBody()->getAngularVelocity().w());
							ImGui::Text("Acceleration: %f, %f, %f, %f\n", rb->GetRigidBody()->getAcceleration().x(), rb->GetRigidBody()->getAcceleration().y(), rb->GetRigidBody()->getAcceleration().z(), rb->GetRigidBody()->getAcceleration().w());
						}
					}
				}
				/*
				static bool enabled = true;
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
				ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::Checkbox("Checkbox", &enabled);
				*/
			}
			ImGui::EndDock();

			ImGui::BeginDock("Layers", NULL, ImGuiWindowFlags_NoSavedSettings);
			{

			}
			ImGui::EndDock();

			ImGui::BeginDock("Content Browser", NULL, ImGuiWindowFlags_NoSavedSettings);
			if (ImGui::Button("New Entity", { 100, 40 }))
			{
				//std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Math::point(0.0f, -0.5f, -1.5f), this->modelInstance1);
				//commandManager->DoCommand(command);
			}
			ImGui::EndDock();
		}
	}

}
