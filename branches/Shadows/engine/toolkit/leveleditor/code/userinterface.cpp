#include "userinterface.h"
#include "render/server/resourceserver.h"
#include "imgui.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "toolkit/tools/style.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/debugrender/debugserver.h"
#include "render/frame/reflectionpass.h"
#include "render/server/frameserver.h"
#include "application/game/cubemapentity.h"
#include "render/server/lightserver.h"

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

        this->toggleLight = false;
	}

	UserInterface::~UserInterface()
	{
		delete this->selectTool;
		delete this->translateTool;
	}

	void UserInterface::Run()
	{
		static bool showStatistics = false;
		static bool showShaderDebugger = true;

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
					if (ImGui::MenuItem("Shader Debugger", NULL, &showShaderDebugger)) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Map"))
			{
				if (ImGui::BeginMenu("Lighting"))
				{
					if (ImGui::MenuItem("Reload Cubemaps", NULL))
					{
						Render::LightServer::Instance()->RegenerateCubemaps();
					}
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

		if (showShaderDebugger)
		{
			ImGui::Begin("Shader Debugger", &showShaderDebugger, ImGuiWindowFlags_ShowBorders);

			ImGui::SetWindowSize(ImVec2(200.0f, 210.0f), ImGuiSetCond_::ImGuiSetCond_Once);
			ImGui::SetWindowPos(ImVec2(1700.0f, 60.0f), ImGuiSetCond_::ImGuiSetCond_Once);
			
			if (ImGui::Button("RELOAD SHADERS"))
			{
				Render::ShaderServer::Instance()->ReloadShaders();
			}

			Math::vec4 cameraZ = Graphics::MainCamera::Instance()->getView().get_zaxis();

			ImGui::Text("Camera Z axis = %f, %f, %f", cameraZ.x(), cameraZ.y(), cameraZ.z());

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
		//Tools
		if (!ImGui::GetIO().KeyCtrl)
		{
			bool switched = false;
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_Q)))
			{
				this->currentTool = selectTool;
				switched = true;
			}
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_W)))
			{
				this->currentTool = translateTool;
				switched = true;
			}
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_E)))
			{
				//this->currentTool = rotateTool;
				switched = true;
			}
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_R)))
			{
				//this->currentTool = scaleTool;
				switched = true;
			}

			if (switched && application->hit.object != nullptr)
			{
				this->currentTool->UpdateTransform(application->hit.object->GetTransform());
			}
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

			bool switched = false;
			if (ImGui::ImageButton((void*)this->selectToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				this->currentTool = selectTool;
				switched = true;
			}
			if (ImGui::ImageButton((void*)this->translateToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				this->currentTool = translateTool;
				switched = true;
			}
			if (ImGui::ImageButton((void*)this->rotateToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = rotateTool;
				switched = true;
			}
			if (ImGui::ImageButton((void*)this->scaleToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = scaleTool;
				switched = true;
			}
			if (ImGui::ImageButton((void*)this->entityToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = entityTool;
				switched = true;
			}
			if (ImGui::ImageButton((void*)this->brushToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = brushTool;
				switched = true;
			}
			if (ImGui::ImageButton((void*)this->polygonEditTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
			{
				//this->currentTool = polygonEditTool;
				switched = true;
			}

			if (switched && application->hit.object != nullptr)
			{
				this->currentTool->UpdateTransform(application->hit.object->GetTransform());
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

					Game::CubeMapEntity* cm = dynamic_cast<Game::CubeMapEntity*>(application->hit.object);

					if (cm != nullptr)
					{
						ImGui::SliderFloat("InnerRange", &cm->GetCubeMapNode()->InnerScale().x(), 0.00001f, 100.0f, "%.3f", 2.0f);
						ImGui::SliderFloat("OuterRange", &cm->GetCubeMapNode()->OuterScale().x(), 0.00001f, 100.0f, "%.3f", 2.0f);
					}
				}
			}
			ImGui::EndDock();

			ImGui::BeginDock("Layers", NULL, ImGuiWindowFlags_NoSavedSettings);
			{
				Render::ReflectionPass::SSRSettings& settings = Render::FrameServer::Instance()->GetReflectionPass()->GetSSRSettings();

				ImGui::SliderFloat("zThickness", &settings.zThickness, 0.00001f, 1000.0f, "%.3f", 4.0f);
				ImGui::InputFloat("Stride (int)", &settings.stride, 1.0f, 100.0f, 0);
				ImGui::SliderFloat("Jitter", &settings.jitter, 0.0f, 1.0f, "%.3f");
				ImGui::SliderFloat("Max Steps", &settings.maxSteps, 1.0f, 1000.0f, "%.3f", 4.0f);
				ImGui::SliderFloat("Max Distance", &settings.maxDistance, 0.001f, 10000.0f, "%.3f", 4.0f);

				const char* items[] = { "HIGH", "MEDIUM", "LOW"};

				ImGui::Combo("Reflection Quality /n", (int*)&Render::FrameServer::Instance()->GetReflectionPass()->GetReflectionQuality(), items, 3);


                if(Render::LightServer::Instance()->GetNumDirectionalLights() != 0 && !this->toggleLight)
                {
                    Render::LightServer::DirectionalLight& dirSettings = Render::LightServer::Instance()->GetDirectionalLightAtIndex(0);
                    ImGui::Text("Directional Light: 0");
                    ImGui::SliderFloat("Red"  , &dirSettings.color.x(), 0.0f, 1.0f, "%.01f");
                    ImGui::SliderFloat("Blue" , &dirSettings.color.y(), 0.0f, 1.0f, "%.01f");
                    ImGui::SliderFloat("Green", &dirSettings.color.z(), 0.0f, 1.0f, "%.01f");
                    ImGui::SliderFloat("Position-X", &dirSettings.direction.x(), -1.0f, 1.0f, "%.01f");
                    ImGui::SliderFloat("Position-Y", &dirSettings.direction.y(), -1.0f, 1.0f, "%.01f");
                    ImGui::SliderFloat("Position-Z", &dirSettings.direction.z(), -1.0f, 1.0f, "%.01f");
                    Render::LightServer::Instance()->UpdateDirectionalLightBuffer();
                }

				if(this->application->hit.object != nullptr)
				{
					Game::ModelEntitySpotLight* aa = dynamic_cast<Game::ModelEntitySpotLight*>(this->application->hit.object);
					Game::PointLightEntity* bb = dynamic_cast<Game::PointLightEntity*>(this->application->hit.object);
                    this->toggleLight = false;

					if(aa != nullptr)
					{
						if(aa->GetLightType() == Render::LightServer::LightType::Spot)
						{
							char i[50];
							Render::LightServer::SpotLight& settings = Render::LightServer::Instance()->GetSpotLightAtIndex(aa->GetLightIndex());
							sprintf(i, "Spotlight: %i", aa->GetLightIndex());
							ImGui::Text(i);
							ImGui::SliderFloat("Angle" , &settings.angle,     1.0f, 89.9f, "%.1f");
							ImGui::SliderFloat("Length", &settings.length,    1.0f, 50.0f, "%.1f");
							ImGui::SliderFloat("Red"   , &settings.color.x(), 0.0f, 1.0f,  "%.01f");
							ImGui::SliderFloat("Green" , &settings.color.y(), 0.0f, 1.0f,  "%.01f");
							ImGui::SliderFloat("Blue"  , &settings.color.z(), 0.0f, 1.0f,  "%.01f");
							ImGui::SliderFloat("X-Direction"  , &settings.coneDirection.x(), -1.0f, 1.0f, "%.01f");
							ImGui::SliderFloat("Y-Direction"  , &settings.coneDirection.y(), -1.0f, 1.0f, "%.01f");
							ImGui::SliderFloat("Z-Direction"  , &settings.coneDirection.z(), -1.0f, 1.0f, "%.01f");

                            this->toggleLight = true;
						}
					}
					if(bb != nullptr)
					{
						if(bb->GetLightType() == Render::LightServer::LightType::Point)
						{
							char i[50];
							Render::LightServer::PointLight& settings = Render::LightServer::Instance()->GetPointLightAtIndex(bb->GetLightIndex());
							sprintf(i, "Point Light: %i", bb->GetLightIndex());
							ImGui::Text(i);
							ImGui::SliderFloat("Radius", &settings.radiusAndPadding.x(), 1.0f, 50.0f, "%.1f");
							ImGui::SliderFloat("Red"   , &settings.color.x(), 0.0f, 1.0f,  "%.01f");
							ImGui::SliderFloat("Green" , &settings.color.y(), 0.0f, 1.0f,  "%.01f");
							ImGui::SliderFloat("Blue"  , &settings.color.z(), 0.0f, 1.0f,  "%.01f");

                            this->toggleLight = true;
						}
					}
				}
			}
			ImGui::EndDock();
			
			ImGui::BeginDock("Content Browser", NULL, ImGuiWindowFlags_NoSavedSettings);
			if (ImGui::Button("Add CubeMap", { 100, 20 }))
			{
				std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
				commandManager->DoCommand(command);
			}
			
			if (ImGui::Button("Spawn Spotlight", { 125, 20 }))
			{
				std::shared_ptr<Edit::AddSpotlightEntity> command = std::make_shared<Edit::AddSpotlightEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
				commandManager->DoCommand(command);
			}

            if (ImGui::Button("Spawn Point Light", { 125, 20 }))
            {
                std::shared_ptr<Edit::AddPointlightEntity> command = std::make_shared<Edit::AddPointlightEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
                commandManager->DoCommand(command);
            }
            if (ImGui::Button("Spawn Box", { 125, 20 }))
            {
                std::shared_ptr<Edit::AddBox> command = std::make_shared<Edit::AddBox>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/placeholdercube.mdl"));
                commandManager->DoCommand(command);
            }
			ImGui::EndDock();
		}
	}

}
