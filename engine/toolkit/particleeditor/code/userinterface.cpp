#include "UserInterface.h"
#include "application.h"
#include "IO/console.h"
#include "nfd.h"
#include "render/server/resourceserver.h"
#include "imgui_internal.h"
#include "toolkit/contentbrowser/code/application.h"

UserInterface::UserInterface(std::shared_ptr<ParticleEditor::Application> app)
{
	this->application = app;
	this->commandManager = Edit::CommandManager::Instance();

	activeEmitter = 0;
	openPopup = false;
	texturePopup = false;

	newEmittIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/add.png");

	emitterTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/particles/particle_spritesheet_smoke2.tga");
	gridTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/whiteBackground.tga");

	ParticleEditor::EmittersUI em = ParticleEditor::EmittersUI(std::shared_ptr<UserInterface>(this), emUI.Size());
	em.ev.active = true;
	emUI.Append(em);

	for (int i = 0; i < 4; i++)
	{
		edSet.col[i] = 1.f;
	}
	edSet.col[3] = 0.f;
	
	ImGui::LoadDock("engine/toolkit/particleeditor/layout/default.layout");
}


UserInterface::~UserInterface()
{
}

void UserInterface::Run()
{
	DrawDocks();

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
				if (ImGui::MenuItem("GUI Console", NULL, NULL)) { IO::Console::Instance()->Show(); }
				//if (ImGui::MenuItem("Statistics", NULL, &showStatistics)) {}
				//if (ImGui::MenuItem("Shader Debugger", NULL, &showShaderDebugger)) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	ModalWindows();
}


void UserInterface::ExecuteShortcuts()
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


void UserInterface::ShowFileMenu()
{

	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		openPopup = true;
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}

	ImGui::Separator();

	if (ImGui::BeginMenu("Layout"))
	{
		if (ImGui::MenuItem("Save Layout...")) { ImGui::SaveDock("engine/toolkit/particleeditor/layout/default.layout"); }
		if (ImGui::MenuItem("Load Layout...")) { ImGui::LoadDock("engine/toolkit/particleeditor/layout/default.layout"); }
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
	if (ImGui::MenuItem("Quit", "Alt+F4")) { application->Shutdown(true); }

}

void UserInterface::DrawDocks()
{
	ImGui::RootDock(ImVec2(0.0f, 16.0f), ImVec2((float)application->GetWindow()->GetWidth(), (float)application->GetWindow()->GetHeight() - 16.0f));
	{
		ImGui::BeginDock("General Settings", NULL, ImGuiWindowFlags_NoSavedSettings);
		{
			DrawGeneralSettings();
		}
		ImGui::EndDock();
		ImGui::BeginDock("Emitters", NULL, ImGuiWindowFlags_NoSavedSettings);
		{
			DrawEmitters();
		}
		ImGui::EndDock();
		ImGui::BeginDock("Render", NULL, ImGuiWindowFlags_NoSavedSettings);
		{
			DrawRender();
		}
		ImGui::EndDock();
		ImGui::BeginDock("Emitter Settings", NULL, ImGuiWindowFlags_NoSavedSettings);
		{
			DrawEmitterSettings();
		}
		ImGui::EndDock();
		ImGui::BeginDock("Texture", NULL, ImGuiWindowFlags_NoSavedSettings);
		{
			ImGui::SameLine(10);
			ImGui::Image((void*)gridTexture->GetHandle(), 
				ImVec2(ImGui::GetWindowContentRegionWidth()-5, ImGui::GetWindowContentRegionWidth() - 10),
				ImVec2(0, 0), ImVec2(1, 1), ImVec4(edSet.col[0], edSet.col[1], edSet.col[2], edSet.col[3]));
			ImGui::SameLine(10);
			ImGui::Image((void*)emitterTexture->GetHandle(), 
				ImVec2(ImGui::GetWindowContentRegionWidth() - 10, ImGui::GetWindowContentRegionWidth() - 10));

			if(ImGui::TreeNode("Texture Preview"))
			{
				ImGui::Text("Background Color:");
				ImGui::ColorEdit4("##backgroundCol", edSet.col);

				ImGui::TreePop();
			}

			DrawTextureSettings();
		}
		ImGui::EndDock();
	}

}

void UserInterface::ModalWindows()
{
	if (this->openPopup){ ImGui::OpenPopup("OpenFile"); }
	
	if (ImGui::BeginPopupModal("OpenFile", &this->openPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("particle", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);
			//if (this->application->loadedModel != nullptr)
			//{
			//	this->application->loadedModel->Deactivate();
			//}
			//this->selectedNode = nullptr;

			//this->application->loadedModel = Game::ModelEntity::Create();
			//this->application->loadedModel->SetModel(Render::ResourceServer::Instance()->LoadModel(outpath));
			//this->application->loadedModel->SetTransform(Math::mat4::scaling(0.01f, 0.01f, 0.01f));
			//this->application->loadedModel->Activate();

			this->openPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->openPopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->openPopup = false;
		}

		ImGui::EndPopup();
	}
	
}

void UserInterface::DrawTextureSettings()
{
	ImGui::BeginChild("##backgroundtex", ImVec2(ImGui::GetWindowContentRegionWidth(), 150), true);
	{
		ImGui::Text("SpriteSheet"); ImGui::SameLine(140);
		if (ImGui::Checkbox("##texsheet", &emUI[0].settings.spriteSheetTex))
		{
			//emitter->GetRenderBuffer().textureAnimation[3] = int(emitter->GetParticleUISettings().spriteSheetTex);
			//emitter->UpdateUniformBuffer();
		}
		ImGui::Text("Frames per row"); ImGui::SameLine(140);
		ImGui::PushItemWidth(80);
		if (ImGui::InputInt("##fpr1", &emUI[0].settings.framesPerRow, 0, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//emitter->GetRenderBuffer().textureAnimation[0] = emitter->GetParticleUISettings().framesPerRow;
			//emitter->UpdateUniformBuffer();
		}
		ImGui::PopItemWidth();

		ImGui::Text("Total frames"); ImGui::SameLine(140);
		ImGui::PushItemWidth(80);
		if (ImGui::InputInt("##tnf2", &emUI[0].settings.numberOfFrames, 0, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//emitter->GetRenderBuffer().textureAnimation[1] = emitter->GetParticleUISettings().numberOfFrames;
			//emitter->UpdateUniformBuffer();
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
}

void UserInterface::DrawGeneralSettings()
{
	ImGui::BeginChild("##GenSet", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight()-40), true);
	{
		ImGui::Text("Time Scale:");
		ImGui::SameLine();
		ImGui::DragFloat("##TimeScale", &edSet.timeScale, 0.01f, 0.01f, 10.f);
	}
	ImGui::EndChild();
}

void UserInterface::DrawEmitters()
{
	ImGui::BeginChild("##es", ImVec2(ImGui::GetWindowWidth() - 20, 45 * (emUI.Size() <= 8 ? emUI.Size() : 8)), false);
	{
		for (int i = 0; i < emUI.Size(); i++)
		{
			emUI[i].DrawEmitter();
		}
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::BeginChild("##cem", ImVec2(ImGui::GetWindowWidth() - 20, 40));
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 30);
		if (ImGui::ImageButton((void*)newEmittIcon->GetHandle(), ImVec2(20, 20)))
		{
			AddNewEmitter();
		}
	}
	ImGui::EndChild();
}

void UserInterface::DrawRender()
{
	ImGui::SameLine(15);
	ImGui::BeginChild("##render", ImVec2(ImGui::GetWindowWidth() - 30, ImGui::GetWindowHeight() - 40), true);
	{
		const char* pMeshItems[] = { "Billboard", "Mesh" };
		static int pMeshItemsPos = 0;
		ImGui::Text("Particle:"); 
		ImGui::SameLine(160); 
		ImGui::Combo("##pMeshCombo", &pMeshItemsPos, pMeshItems, IM_ARRAYSIZE(pMeshItems));

		ImGui::Text("Texture:");
		ImGui::SameLine(160);
		ImGui::InputText("##texture", (char*)emUI[activeEmitter].settings.texName.c_str(), 512, ImGuiInputTextFlags_ReadOnly);
		//ImGui::ImageButton();

		const char* pBlendItems[] = { "Additive", "Alpha", "Multiply" };
		static int pBlendItemsPos = 0;
		ImGui::Text("Blend type:");
		ImGui::SameLine(160);
		ImGui::Combo("##pBlendCombo", &pBlendItemsPos, pBlendItems, IM_ARRAYSIZE(pBlendItems));

		ImGui::Separator();
	
		const char* pColorItems[] = { "Solid", "???", "???" };
		static int pColorItemsPos = 0;
		ImGui::Text("Start Color:");
		ImGui::SameLine(160);
		ImGui::Combo("##pColorCombo", &pColorItemsPos, pColorItems, IM_ARRAYSIZE(pColorItems));

		GLuint texId = 0;
		ImGui::Text("Color:");
		ImGui::SameLine(160);
		ImGui::ColorButton(ImVec4(edSet.col[0], edSet.col[1], edSet.col[2], edSet.col[3]), ImVec2(415, 0));

		ImGui::Separator();

	}
	ImGui::EndChild();
}

void UserInterface::DrawEmitterSettings()
{
	ImGui::SameLine(15);
	ImGui::BeginChild("##emitterSettings", ImVec2(ImGui::GetWindowWidth() - 30, ImGui::GetWindowHeight() - 40), true);
	{
		/*ImGui::Begin("ColorPicker##test");
		float c[4];
		ImGui::ColorPicker4("##ColorPick", c);
		ImGui::End();*/
	}
	ImGui::EndChild();
}

void UserInterface::AddNewEmitter()
{
	ParticleEditor::EmittersUI tempEm(std::shared_ptr<UserInterface>(this), emUI.Size());
	emUI.Append(tempEm);
}


void UserInterface::DuplicateEmitter(ParticleEditor::EmittersUI newEmitter)
{
	ParticleEditor::EmittersUI tempEm(std::shared_ptr<UserInterface>(this),emUI.Size());
	tempEm.ev = newEmitter.ev;
	tempEm.ev.active = false;
	emUI.Append(tempEm);
}

void UserInterface::UpdateActiveEmitter(int id)
{
	for (int i = 0; i < emUI.Size(); i++)
	{
		if (i == id)
			continue;

		emUI[i].ev.active = false;
	}
	//emUI[id] use to set the values in the editor
}
