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
#include "rotatetool.h"
#include "render/debugrender/debugrenderer.h"
#include "imgui_internal.h"
#include "render/particlesystem/particlefile.h"


#define CONSOLE_BUFFER_SIZE 8096

namespace Toolkit
{
	UserInterface::UserInterface(LevelEditor::Application* app)
	{
		this->application = app;

		this->selectTool = new Tools::SelectTool();
		this->translateTool = new Tools::TranslateTool();
		this->rotateTool = new Tools::RotateTool();
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
		delete this->rotateTool;
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
				if (ImGui::BeginMenu("Geometry"))
				{
					if (ImGui::MenuItem("Show Geometry Proxies", NULL, &this->application->renderGeoProxies)) {}
					if (ImGui::MenuItem("New Geometry Proxy", NULL)) 
					{
						std::shared_ptr<Edit::AddGeometryProxyEntity> command = std::make_shared<Edit::AddGeometryProxyEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
						commandManager->DoCommand(command);
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
				this->currentTool = rotateTool;
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
				this->currentTool = rotateTool;
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
		
		static Math::mat4 delta;

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

							delta = this->currentTool->GetDeltaMatrix();
							Math::mat4 objTransform = this->application->hit.object->GetTransform();
							
							this->application->hit.object->SetTransform(Math::mat4::multiply(delta, objTransform));
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
					
					ImGui::Text("Rotate:");

					float xRot = 0;	float yRot = 0;	float zRot = 0;

					ImGui::SliderFloat("X", &xRot, -0.05f, 0.05f, "%.3f", 1.0f);
					ImGui::SliderFloat("Y", &yRot, -0.05f, 0.05f, "%.3f", 1.0f);
					ImGui::SliderFloat("Z", &zRot, -0.05f, 0.05f, "%.3f", 1.0f);

					Math::mat4 transform = Math::mat4::multiply(Math::mat4::rotationyawpitchroll(yRot, xRot, zRot), application->hit.object->GetTransform());
					application->hit.object->SetTransform(transform);

					float yaw =   (float)atan2(-transform.getrow0().z(), transform.getrow0().x());
					float pitch = (float)asin(transform.getrow0().y());
					float roll =  (float)atan2(-transform.getrow2().y(), transform.getrow1().y());

					ImGui::Text("Pitch : %f | Yaw : %f | Roll : %f", pitch, yaw, roll);
					ImGui::Separator();

					// CUBEMAPS --------------------------------------------
					Game::CubeMapEntity* cm = dynamic_cast<Game::CubeMapEntity*>(application->hit.object);

					if (cm != nullptr)
					{
						ImGui::SliderFloat("InnerRange", &cm->GetCubeMapNode()->InnerScale().x(), 0.00001f, 100.0f, "%.3f", 2.0f);
						ImGui::SliderFloat("OuterRange", &cm->GetCubeMapNode()->OuterScale().x(), 0.00001f, 100.0f, "%.3f", 2.0f);
					}

					Game::GeometryProxyEntity* gpe = dynamic_cast<Game::GeometryProxyEntity*>(application->hit.object);

					if (gpe != nullptr)
					{
						Math::vec4 scale = gpe->GetScale();

						ImGui::SliderFloat("Scale X", &scale.x(), 1.0f, 100.0f, "%.3f", 3.0f);
						ImGui::SliderFloat("Scale Y", &scale.y(), 1.0f, 100.0f, "%.3f", 3.0f);
						ImGui::SliderFloat("Scale Z", &scale.z(), 1.0f, 100.0f, "%.3f", 3.0f);

						gpe->SetScale(scale);

						if (ImGui::Button("Add CubeMap To Proxy", { 100, 40 }))
						{
							std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
							commandManager->DoCommand(command);
							command->entity->SetGeometryProxy(gpe->GetGeometryProxy());
						}
					}

					//LIGHTS --------------------------------------------------

					Game::ModelEntitySpotLight* aa = dynamic_cast<Game::ModelEntitySpotLight*>(this->application->hit.object);
					Game::PointLightEntity* bb = dynamic_cast<Game::PointLightEntity*>(this->application->hit.object);

					if (aa != nullptr)
					{
						ImGui::Text("Spot Light");

						float angle = aa->GetSpotLightAngle();
						if (ImGui::SliderFloat("Angle", &angle, 1.0f, 90.0f, "%.1f"))
						{
							aa->SetSpotLightAngle(angle);
						}
						float length = aa->GetSpotLightLength();
						if (ImGui::SliderFloat("Length", &length, 1.0f, 50.0f, "%.1f"))
						{
							aa->SetSpotLightLength(length);
						}

						Math::vec4 color = aa->GetSpotLightColor();
						if (ImGui::SliderFloat("Red", &color.x(), 0.0f, 1.0f, "%.01f"))
						{
							aa->SetSpotLightColor(color);
						}
						if (ImGui::SliderFloat("Green", &color.y(), 0.0f, 1.0f, "%.01f"))
						{
							aa->SetSpotLightColor(color);
						}
						if (ImGui::SliderFloat("Blue", &color.z(), 0.0f, 1.0f, "%.01f"))
						{
							aa->SetSpotLightColor(color);
						}

					}
					if (bb != nullptr)
					{
						ImGui::Text("Point Light");

						float radius = bb->GetPointLightRadius();
						if (ImGui::SliderFloat("Radius", &radius, 1.0f, 50.0f, "%.1f"))
						{
							bb->SetPointLightRadius(radius);
						}

						Math::vec4 color = bb->GetPointLightColor();
						if (ImGui::SliderFloat("Red", &color.x(), 0.0f, 1.0f, "%.01f"))
						{
							bb->SetPointLightColor(color);
						}
						if (ImGui::SliderFloat("Green", &color.y(), 0.0f, 1.0f, "%.01f"))
						{
							bb->SetPointLightColor(color);
						}
						if (ImGui::SliderFloat("Blue", &color.z(), 0.0f, 1.0f, "%.01f"))
						{
							bb->SetPointLightColor(color);
						}
					}

				}
			}
			ImGui::EndDock();

			ImGui::BeginDock("Particle Settings", NULL, ImGuiWindowFlags_NoSavedSettings);
			{
				this->particleCount = 0;
				if (ImGui::Button("Save"))
				{
					Particles::ParticleFile::Instance()->SaveParticle("testParticle");
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Saves the current particles.\nSaves all the currently appended emitters\nthen removes them from the list");
					ImGui::EndTooltip();
				}
				for (int i = 0; i < application->particleList.Size(); ++i)
				{
					ParticlesSettings(application->particleList[i]->GetEmitter());
					this->particleCount++;
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

				ImGui::Combo("Reflection Quality", (int*)&Render::FrameServer::Instance()->GetReflectionPass()->GetReflectionQuality(), items, 3);
			}
			ImGui::EndDock();
			
			ImGui::BeginDock("Content Browser", NULL, ImGuiWindowFlags_NoSavedSettings);
			if (ImGui::Button("Add CubeMap", { 100, 40 }))
			{
				std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
				commandManager->DoCommand(command);
			}

			if (ImGui::Button("Add Spotlight", { 125, 40 }))
			{
				std::shared_ptr<Edit::AddSpotlightEntity> command = std::make_shared<Edit::AddSpotlightEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
				commandManager->DoCommand(command);
			}

            if (ImGui::Button("Add Point Light", { 125, 40 }))
            {
                std::shared_ptr<Edit::AddPointlightEntity> command = std::make_shared<Edit::AddPointlightEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
                commandManager->DoCommand(command);
            }
			ImGui::EndDock();
		}
	}


void UserInterface::ParticlesSettings(std::shared_ptr<Property::ParticleEmitter> emitter)
{
	std::string id = "Particle "+std::to_string(particleCount);
	if (ImGui::CollapsingHeader(id.c_str()))
	{
		
		id = "Append Emitter##" + std::to_string(particleCount);
		if (ImGui::Button(id.c_str()))
		{
			Particles::ParticleFile::Instance()->AppendEmitter(emitter);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Append the emitter into the current particle.\nUsed when saving");
			ImGui::EndTooltip();
		}
		ImGui::SameLine(120);
		id = "name##" + std::to_string(particleCount);
		ImGui::InputText(id.c_str(), (char*)emitter->GetEmitterName().c_str(), 32);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Emitter name");
			ImGui::EndTooltip();
		}
		
		id = "Base Settings##" + std::to_string(particleCount);
		if (ImGui::TreeNode(id.c_str()))
		{

			emitter->GetParticleUISettings().acc = emitter->GetState().accLife;
			emitter->GetParticleUISettings().color = emitter->GetState().color;
			id = "Velocity##" + std::to_string(particleCount);
			if (ImGui::TreeNode(id.c_str()))
			{
				ImGui::Text("Randomize"); ImGui::SameLine(120);
				id = "##vb" + std::to_string(particleCount);
				ImGui::Checkbox(id.c_str(), &emitter->GetParticleUISettings().vecRand);
				ImGui::Text("Min"); ImGui::SameLine(100);
				id = "##v1" + std::to_string(particleCount);
				if (ImGui::SliderFloat(id.c_str(), (float*)&emitter->GetParticleUISettings().vel, 0, 30, "%.3f", 5))
				{
					if (emitter->GetParticleUISettings().vel > emitter->GetParticleUISettings().vel2)
					{
						float temp = emitter->GetParticleUISettings().vel;
						emitter->GetParticleUISettings().vel = emitter->GetParticleUISettings().vel2;
						emitter->GetParticleUISettings().vel2 = emitter->GetParticleUISettings().vel;
					}
					Particles::ParticleSystem::Instance()->UpdateParticleVelocity(emitter, emitter->GetParticleUISettings().vel, emitter->GetParticleUISettings().vel2, emitter->GetParticleUISettings().radius, emitter->GetParticleUISettings().shapes, emitter->GetParticleUISettings().vecRand);
				}
				if (emitter->GetParticleUISettings().vecRand)
				{
					ImGui::Text("Max"); ImGui::SameLine(100);
					id = "##v2" + std::to_string(particleCount);
					if (ImGui::SliderFloat(id.c_str(), (float*)&emitter->GetParticleUISettings().vel2, 0, 30, "%.3f", 5))
					{
						if (emitter->GetParticleUISettings().vel > emitter->GetParticleUISettings().vel2)
						{
							float temp = emitter->GetParticleUISettings().vel;
							emitter->GetParticleUISettings().vel = emitter->GetParticleUISettings().vel2;
							emitter->GetParticleUISettings().vel2 = emitter->GetParticleUISettings().vel;
						}
						Particles::ParticleSystem::Instance()->UpdateParticleVelocity(emitter, emitter->GetParticleUISettings().vel, emitter->GetParticleUISettings().vel2, emitter->GetParticleUISettings().radius, emitter->GetParticleUISettings().shapes, emitter->GetParticleUISettings().vecRand);
					}
				}
				ImGui::TreePop();
			}
			id = "Lifetime##" + std::to_string(particleCount);
			if (ImGui::TreeNode(id.c_str()))
			{
				ImGui::Text("Randomize"); ImGui::SameLine(120);
				id = "##lb" + std::to_string(particleCount);
				ImGui::Checkbox(id.c_str(), &emitter->GetParticleUISettings().lifeTimeRand);
				ImGui::Text("Min"); ImGui::SameLine(100);
				id = "##l1" + std::to_string(particleCount);
				if (ImGui::SliderFloat(id.c_str(), (float*)&emitter->GetParticleUISettings().acc[3], 0, 100, "%.3f", 5))
				{
					if (emitter->GetParticleUISettings().acc[3] > emitter->GetParticleUISettings().acc2[3])
					{
						float temp = emitter->GetParticleUISettings().acc[3];
						emitter->GetParticleUISettings().acc[3] = emitter->GetParticleUISettings().acc2[3];
						emitter->GetParticleUISettings().acc2[3] = emitter->GetParticleUISettings().acc[3];
					}
					Particles::ParticleSystem::Instance()->UpdateParticleLifetime(emitter, emitter->GetParticleUISettings().acc[3], emitter->GetParticleUISettings().acc2[3], emitter->GetParticleUISettings().lifeTimeRand);
				}
				if (emitter->GetParticleUISettings().lifeTimeRand)
				{
					ImGui::Text("Max"); ImGui::SameLine(100);
					id = "##l2" + std::to_string(particleCount);
					if (ImGui::SliderFloat(id.c_str(), (float*)&emitter->GetParticleUISettings().acc2[3], 0, 100, "%.3f", 5))
					{
						if (emitter->GetParticleUISettings().acc[3] > emitter->GetParticleUISettings().acc2[3])
						{
							float temp = emitter->GetParticleUISettings().acc[3];
							emitter->GetParticleUISettings().acc[3] = emitter->GetParticleUISettings().acc2[3];
							emitter->GetParticleUISettings().acc2[3] = emitter->GetParticleUISettings().acc[3];
						}
						Particles::ParticleSystem::Instance()->UpdateParticleLifetime(emitter, emitter->GetParticleUISettings().acc[3], emitter->GetParticleUISettings().acc2[3], emitter->GetParticleUISettings().lifeTimeRand);
					}
				}
				ImGui::TreePop();
			}
			id = "Acceleration##" + std::to_string(particleCount);
			if (ImGui::TreeNode(id.c_str()))
			{
				ImGui::Text("Randomize"); ImGui::SameLine(120);
				id = "##ab" + std::to_string(particleCount);
				ImGui::Checkbox(id.c_str(), &emitter->GetParticleUISettings().accRand);
				ImGui::Text("Min"); ImGui::SameLine(100);
				id = "##amin" + std::to_string(particleCount);
				if (ImGui::SliderFloat3(id.c_str(), (float*)&emitter->GetParticleUISettings().acc, -50, 50, "%.3f", 5))
				{
					if (Math::vec4::greater3_all(emitter->GetParticleUISettings().acc, emitter->GetParticleUISettings().acc2))
					{
						Math::vec4 temp = emitter->GetParticleUISettings().acc;
						emitter->GetParticleUISettings().acc = emitter->GetParticleUISettings().acc2;
						emitter->GetParticleUISettings().acc2 = emitter->GetParticleUISettings().acc;
					}
					Particles::ParticleSystem::Instance()->UpdateParticleAcceleration(emitter, emitter->GetParticleUISettings().acc, emitter->GetParticleUISettings().acc2, emitter->GetParticleUISettings().accRand);
				}
				if (emitter->GetParticleUISettings().accRand)
				{
					ImGui::Text("Max"); ImGui::SameLine(100);
					id = "##amax" + std::to_string(particleCount);
					if (ImGui::SliderFloat3(id.c_str(), (float*)&emitter->GetParticleUISettings().acc2, -50, 50, "%.3f", 5))
					{
						if (Math::vec4::greater3_all(emitter->GetParticleUISettings().acc, emitter->GetParticleUISettings().acc2))
						{
							Math::vec4 temp = emitter->GetParticleUISettings().acc;
							emitter->GetParticleUISettings().acc = emitter->GetParticleUISettings().acc2;
							emitter->GetParticleUISettings().acc2 = emitter->GetParticleUISettings().acc;
						}
						Particles::ParticleSystem::Instance()->UpdateParticleAcceleration(emitter, emitter->GetParticleUISettings().acc, emitter->GetParticleUISettings().acc2, emitter->GetParticleUISettings().accRand);
					}
				}

				ImGui::TreePop();
			}
			id = "Color##" + std::to_string(particleCount);
			if (ImGui::TreeNode(id.c_str()))
			{
				ImGui::Text("Randomize"); ImGui::SameLine(120);
				id = "##cb" + std::to_string(particleCount);
				ImGui::Checkbox(id.c_str(), &emitter->GetParticleUISettings().colorRand);
				ImGui::Text("Min"); ImGui::SameLine(100);
				id = "##c1" + std::to_string(particleCount);
				if (ImGui::ColorEdit4(id.c_str(), (float*)&emitter->GetParticleUISettings().color, ImGuiColorEditFlags_Alpha))
				{
					if (Math::vec4::greater3_all(emitter->GetParticleUISettings().color, emitter->GetParticleUISettings().color2))
					{
						Math::vec4 temp = emitter->GetParticleUISettings().color;
						emitter->GetParticleUISettings().color = emitter->GetParticleUISettings().color2;
						emitter->GetParticleUISettings().color2 = emitter->GetParticleUISettings().color;
					}
					Particles::ParticleSystem::Instance()->UpdateParticleColor(emitter, emitter->GetParticleUISettings().color, emitter->GetParticleUISettings().color2, emitter->GetParticleUISettings().colorRand);
				}
				if (emitter->GetParticleUISettings().colorRand)
				{
					ImGui::Text("Max"); ImGui::SameLine(100);
					id = "##c2" + std::to_string(particleCount);
					if (ImGui::ColorEdit4(id.c_str(), (float*)&emitter->GetParticleUISettings().color2, ImGuiColorEditFlags_Alpha))
					{
						if (Math::vec4::greater3_all(emitter->GetParticleUISettings().color, emitter->GetParticleUISettings().color2))
						{
							Math::vec4 temp = emitter->GetParticleUISettings().color;
							emitter->GetParticleUISettings().color = emitter->GetParticleUISettings().color2;
							emitter->GetParticleUISettings().color2 = emitter->GetParticleUISettings().color;
						}
						//ImGui::ColorConvertHSVtoRGB()
						Particles::ParticleSystem::Instance()->UpdateParticleColor(emitter, emitter->GetParticleUISettings().color, emitter->GetParticleUISettings().color2, emitter->GetParticleUISettings().colorRand);
					}
				}
				ImGui::TreePop();
			}
			id = "Size##" + std::to_string(particleCount);
			if (ImGui::TreeNode(id.c_str()))
			{
				ImGui::Text("Start"); ImGui::SameLine(100);
				id = "##s1" + std::to_string(particleCount);
				if (ImGui::SliderFloat(id.c_str(), (float*)&emitter->GetParticleUISettings().startSize, 0, 20, "%.3f", 5))
				{

					Particles::ParticleSystem::Instance()->UpdateParticleSize(emitter, emitter->GetParticleUISettings().startSize, emitter->GetParticleUISettings().endSize);
				}
				ImGui::Text("End"); ImGui::SameLine(100);
				id = "##s2" + std::to_string(particleCount);
				if (ImGui::SliderFloat(id.c_str(), (float*)&emitter->GetParticleUISettings().endSize, 0, 20, "%.3f", 5))
				{
					Particles::ParticleSystem::Instance()->UpdateParticleSize(emitter, emitter->GetParticleUISettings().startSize, emitter->GetParticleUISettings().endSize);
				}

				ImGui::TreePop();
			}

			ImGui::Text("Max particles"); ImGui::SameLine(130);
			id = "##mp" + std::to_string(particleCount);
			ImGui::PushItemWidth(150);
			if (ImGui::SliderInt(id.c_str(), &emitter->GetParticleUISettings().numParticles, 1, 1000000))
			{
				Particles::ParticleSystem::Instance()->GetEmitterBuffer(emitter->GetParticleUISettings().numParticles, emitter, emitter->GetEmitterBuffer());
			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Emitter Shape Settings"))
		{
			const char* items[] = { "Cone", "Sphere", "Hemisphere" };
			static int item2 = 0;
			ImGui::Text("Emitter Shape"); ImGui::SameLine(130);
			id = "##cbs" + std::to_string(particleCount);
			ImGui::PushItemWidth(170);
			if (ImGui::Combo(id.c_str(), &item2, items, IM_ARRAYSIZE(items)))
			{
				emitter->GetParticleUISettings().shapes = (Particles::EmitterShapes)item2;
			}
			ImGui::PopItemWidth();
			if (emitter->GetParticleUISettings().shapes == Particles::CONE)
			{
				ImGui::Text("Cone Radius"); ImGui::SameLine(130);
				id = "##cRadius" + std::to_string(particleCount);
				ImGui::PushItemWidth(150);
				if (ImGui::SliderFloat(id.c_str(), &emitter->GetParticleUISettings().radius, 0.001, 10, "%.3f", 5))
				{
					Particles::ParticleSystem::Instance()->UpdateParticleVelocity(emitter, emitter->GetParticleUISettings().vel, emitter->GetParticleUISettings().vel2, emitter->GetParticleUISettings().radius, emitter->GetParticleUISettings().shapes, emitter->GetParticleUISettings().vecRand);
				}
				ImGui::PopItemWidth();
			}
			else if (emitter->GetParticleUISettings().shapes == Particles::SPHERE)
			{
				ImGui::Text("Sphere Radius"); ImGui::SameLine(130);
				id = "##sRadius" + std::to_string(particleCount);
				ImGui::PushItemWidth(150);
				if (ImGui::SliderFloat(id.c_str(), &emitter->GetParticleUISettings().radius, 0.001, 10, "%.3f", 5))
				{
					Particles::ParticleSystem::Instance()->UpdateParticleVelocity(emitter, emitter->GetParticleUISettings().vel, emitter->GetParticleUISettings().vel2, emitter->GetParticleUISettings().radius, emitter->GetParticleUISettings().shapes, emitter->GetParticleUISettings().vecRand);
				}
				ImGui::PopItemWidth();
			}
			else if (emitter->GetParticleUISettings().shapes == Particles::HEMISPHERE)
			{
				ImGui::Text("Hemisphere Radius"); ImGui::SameLine(160);
				id = "##hsRadius" + std::to_string(particleCount);
				ImGui::PushItemWidth(150);
				if (ImGui::SliderFloat(id.c_str(), &emitter->GetParticleUISettings().radius, 0.001, 10, "%.3f", 5))
				{
					Particles::ParticleSystem::Instance()->UpdateParticleVelocity(emitter, emitter->GetParticleUISettings().vel, emitter->GetParticleUISettings().vel2, emitter->GetParticleUISettings().radius, emitter->GetParticleUISettings().shapes, emitter->GetParticleUISettings().vecRand);
				}
				ImGui::PopItemWidth();
			}
			
			//Debug::DebugRenderer::Instance()->DrawCone(Math::point(0, 0, 0), Math::quaternion::rotationyawpitchroll(0.0f, 3.14f, 0.0f), application->emitter->GetParticleUISettings()tings.radius, 1.0f, application->emitter->GetParticleUISettings()tings.color, Debug::RenderMode::WireFrame, 2.0f);
			ImGui::TreePop();
		}
	}	

}

}
