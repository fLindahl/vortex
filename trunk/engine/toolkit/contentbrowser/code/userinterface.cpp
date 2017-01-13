#include "userinterface.h"
#include "render/server/resourceserver.h"
#include "imgui.h"
#include "application.h"
#include "render/server/frameserver.h"
#include "toolkit/tools/style.h"
#include "nfd.h"
#include "render/resources/surface.h"

namespace Toolkit
{
	UserInterface::UserInterface(ContentBrowser::Application* app)
	{
		this->application = app;
		this->savePath = "";
		this->browseButtonTextureHandle = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/leveleditor/resources/textures/texture.png")->GetHandle();
		//Setup ImGui Stuff
		SetupImGuiStyle();
		ImGui::LoadDock("engine/toolkit/contentbrowser/layout/default.layout");
	}

	UserInterface::~UserInterface()
	{

	}

	void UserInterface::Run()
	{
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
				//if (ImGui::MenuItem("Undo", "CTRL+Z")) { }
				//if (ImGui::MenuItem("Redo", "CTRL+Y")) { }  // Disabled item
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
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();

			if (this->openFilePopup){ ImGui::OpenPopup("OpenFile"); }
			if (this->confirmNewModelPopup){ ImGui::OpenPopup("ConfirmNewModel"); }

			if (ImGui::BeginPopupModal("OpenFile", &this->openFilePopup))
			{
				nfdchar_t* outpath;
				nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outpath);

				if (result = NFD_OKAY)
				{
					printf("path: %s\n", outpath);
					if (this->application->loadedModel != nullptr)
					{
						this->application->loadedModel->Deactivate();
					}

					this->application->loadedModel = std::make_shared<Game::ModelEntity>();
					
					this->application->loadedModel->SetModel(Render::ResourceServer::Instance()->LoadModel(outpath));

					this->application->loadedModel->Activate();

					this->openFilePopup = false;
					free(outpath);
				}
				else if (result == NFD_CANCEL)
				{
					printf("User pressed cancel.\n");
				}
				else 
				{
					printf("Error: %s\n", NFD_GetError());
					assert(false);
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("ConfirmNewModel", &this->confirmNewModelPopup))
			{
				ImGui::SetWindowSize(ImVec2(300, 300), ImGuiSetCond_::ImGuiSetCond_Once);

				ImGui::Text("Are you sure you want to create a new model?");
				ImGui::Text("Any unsaved progress will be lost.");

				if (ImGui::Button("Yes"))
				{
					this->application->NewModel();
					this->confirmNewModelPopup = false;
				}
				if (ImGui::Button("No"))
				{
					this->confirmNewModelPopup = false;
				}

				ImGui::EndPopup();
			}
		}
	}

	void UserInterface::ShowFileMenu()
	{
		if (ImGui::BeginMenu("New..."))
		{
			if (ImGui::MenuItem("Model"))
			{
				NewModel();
			}
			if (ImGui::MenuItem("Surface"))
			{
				NewSurface();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Open", "Ctrl+O")) 
		{
			this->openFilePopup = true;
		}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("example1.map");
			ImGui::MenuItem("example2.map");
			ImGui::MenuItem("example3.map");
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			this->SaveModel();
		}
		if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) 
		{
			this->SaveModel(true);
		}
		ImGui::Separator();

		if (ImGui::BeginMenu("Layout"))
		{
			if (ImGui::MenuItem("Save Layout...")) { ImGui::SaveDock("engine/toolkit/contentbrowser/layout/default.layout"); }
			if (ImGui::MenuItem("Load Layout...")) { ImGui::LoadDock("engine/toolkit/contentbrowser/layout/default.layout"); }
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Quit", "Alt+F4")) { application->shutdown = true; }
	}

	void UserInterface::ExecShortCuts()
	{
		if ((ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_S))))
		{
			this->SaveModel();
		}
	}


	void UserInterface::RenderDocks()
	{		
		ImGui::RootDock(ImVec2(0.0f, 16.0f), ImVec2((float)application->window->GetWidth(), (float)application->window->GetHeight() - 16.0f));
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

			ImGui::BeginDock("Inspector", NULL, ImGuiWindowFlags_NoSavedSettings);
			{
				if (this->application->loadedModel != nullptr)
				{
					ImGui::Text("Model: %s", this->application->loadedModel->GetGraphicsProperty()->getModelInstance()->GetName().c_str());
					ImGui::Text("Mesh: %s", this->application->loadedModel->GetGraphicsProperty()->getModelInstance()->GetMesh()->GetPath().c_str());
					
					if (ImGui::Button("Browse...", ImVec2(48, 16)))
					{
						nfdchar_t* outpath;
						nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outpath);

						if (result = NFD_OKAY)
						{
							printf("mesh path: %s\n", outpath);
							
							this->application->loadedModel->Deactivate();
							
							this->application->loadedModel->GetGraphicsProperty()->getModelInstance()->SetMesh(outpath);

							this->application->loadedModel->Activate();

							this->openFilePopup = false;
							free(outpath);
						}
						else if (result == NFD_CANCEL)
						{
							//Do nothing
						}
						else
						{
							printf("Error: %s\n", NFD_GetError());
							assert(false);
						}
					}

					ImGui::BeginChild("child", ImVec2(0, 500), true);
					{
						//Unique identifier
						uint i = 2014235;
						for (auto node : this->application->loadedModel->GetGraphicsProperty()->getModelInstance()->GetModelNodes())
						{
							ImGui::Text("Node: %i | Surface: %s", node->primitiveGroup, node->surface->GetPath().c_str());
							ImGui::PushID(i);
							if (ImGui::ImageButton((void*)this->browseButtonTextureHandle, ImVec2(16, 16)))
							{
								nfdchar_t* outpath;
								nfdresult_t result = NFD_OpenDialog("surface", NULL, &outpath);

								if (result = NFD_OKAY)
								{
									printf("surface path: %s\n", outpath);

									node->surface->RemoveModelNode(node);
									auto surface = Render::ResourceServer::Instance()->LoadSurface(outpath);
									surface->AppendModelNode(node);
									node->surface = surface.get();

									this->openFilePopup = false;
									free(outpath);
								}
								else if (result == NFD_CANCEL)
								{
									//Do nothing
								}
								else
								{
									printf("Error: %s\n", NFD_GetError());
									assert(false);
								}
							}
							ImGui::PopID();
							i++;
							ImGui::Separator();
						}
						ImGui::EndChild();
					}


				}
			}
			ImGui::EndDock();
		}
	}

	void UserInterface::NewModel()
	{
		if (this->application->loadedModel != nullptr)
		{
			this->confirmNewModelPopup = true;
		}
		else
		{
			this->application->NewModel();
		}
	}

	void UserInterface::SaveModel(bool newPath)
	{
		if (this->savePath.empty() || newPath)
		{
			nfdchar_t* outpath = nullptr;
			nfdresult_t result = NFD_SaveDialog("mdl", NULL, &outpath);

			std::string newFilePath = outpath;
			
			free(outpath);

			if (newFilePath.substr(newFilePath.find_last_of(".") + 1) != "mdl") 
			{
				newFilePath.append(".mdl");
			}			

			if (result = NFD_OKAY)
			{
				this->savePath = newFilePath;
			}
			else if (result == NFD_CANCEL)
			{
				//Do nothing!
				return;
			}
			else
			{
				printf("Error: %s\n", NFD_GetError());
				assert(false);
				return;
			}
		}		
		printf("Save Path: %s\n", this->savePath.c_str());
		this->application->SaveModel(this->savePath.c_str());		
	}

	void UserInterface::NewSurface()
	{

	}
}
