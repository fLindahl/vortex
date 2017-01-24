#include "userinterface.h"
#include "render/server/resourceserver.h"
#include "imgui.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "toolkit/tools/style.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/debugrender/debugserver.h"

#include "basetool.h"
#include "selecttool.h"
#include "translatetool.h"
#include "render/debugrender/debugrenderer.h"
#include "imgui_internal.h"

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
		
		Debug::DebugServer::Instance()->ImGuiDebugBar();

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
		const float toolbarWidth = 52.0f;
		const float toolButtonSize = 32.0f;

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
			ImGui::SetWindowSize(ImVec2(toolbarWidth, (float)application->window->GetHeight() - 16.0f), ImGuiSetCond_Once);
			ImGui::SetWindowPos(ImVec2(0.0f, 16.0f), ImGuiSetCond_Once);

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
		
		ImGui::RootDock(ImVec2(toolbarWidth, 16.0f), ImVec2((float)application->window->GetWidth() - toolbarWidth, (float)application->window->GetHeight() - 16));
		{
			ImGui::BeginDock("3D View", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				ImVec2 dockSize = ImGui::GetWindowSize();
				ImGui::Image((void*)Render::RenderDevice::Instance()->GetFinalColorBuffer(), dockSize);

				this->currentTool->UpdateHandlePositions();
				this->currentTool->Render();

				if (ImGui::IsItemHovered())
				{
					if (application->hit.object != nullptr)
					{
						if (ImGui::GetIO().MouseClicked[0])
						{
							this->currentTool->LeftDown();
							this->currentTool->UpdateTransform(application->hit.object->GetTransform());
							if (this->currentTool->GetCurrentHandle() == Tools::TransformHandle::NONE)
							{
								application->DoPicking();
							}
						}
						if (ImGui::GetIO().MouseReleased[0])
						{
							this->currentTool->LeftUp();
						}
						if (ImGui::GetIO().MouseDown[0])
						{
							this->currentTool->Drag();

							const Math::mat4& delta = this->currentTool->GetDeltaMatrix();
							Math::mat4 objTransform = this->application->hit.object->GetTransform();
							
							this->application->hit.object->SetTransform(Math::mat4::multiply(objTransform, delta));
							this->currentTool->UpdateTransform(application->hit.object->GetTransform());
						}
					}
					else
					{
						application->DoPicking();
						if (application->hit.object != nullptr)
						{
							this->currentTool->UpdateTransform(application->hit.object->GetTransform());
						}
					}
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
			}
			ImGui::EndDock();

			ImGui::BeginDock("Particle Settings", NULL, ImGuiWindowFlags_NoSavedSettings);
			{
				
				if (ImGui::CollapsingHeader("Base Settings"))
				{
					const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK" };
					static int item2 = 0;
					ImGui::Combo("combo scroll", &item2, items, IM_ARRAYSIZE(items));
					if (ImGui::TreeNode("Color"))
					{
						//ImGui::ColorPicker((float*)&application->pSettings.color, true);
						ImGui::TreePop();
					}
						
				}
				if (ImGui::CollapsingHeader("Emitter Shape"))
				{
					//ImGui::InputFloat("Cone Radius", &application->pSettings.radius, 0.001f, 5.0f);
					//ImGui::SliderFloat("Cone Length", &application->pSettings.length, 0.001, 5, "%.3f");
					//Debug::DebugRenderer::Instance()->DrawCone(Math::point(0, 0, 0), Math::quaternion::rotationyawpitchroll(0.0f, 3.14f, 0.0f), application->pSettings.radius, 1.0f, application->pSettings.color, Debug::RenderMode::WireFrame, 2.0f);
				}
			}
			ImGui::EndDock();

			ImGui::BeginDock("Layers", NULL, ImGuiWindowFlags_NoSavedSettings);
			{

			}
			ImGui::EndDock();
			
			ImGui::BeginDock("Content Browser", NULL, ImGuiWindowFlags_NoSavedSettings);
			if (ImGui::Button("New Entity", { 100, 40 }))
			{
				std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Math::point(0.0f, -0.5f, -1.5f), Render::ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));
				commandManager->DoCommand(command);
			}
			ImGui::EndDock();
		}
	}

}
