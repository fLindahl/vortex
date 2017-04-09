#include "UserInterface.h"
#include "application.h"
#include "IO/console.h"
#include "render/server/resourceserver.h"
#include "imgui_internal.h"

UserInterface::UserInterface(std::shared_ptr<ParticleEditor::Application> app)
{
	this->application = app;
	this->commandManager = Edit::CommandManager::Instance();

	newEmittIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/add.png");

	emitterTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/particles/particle_spritesheet_smoke2.tga");
	gridTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/whiteBackground.tga");

	ParticleEditor::EmittersUI em = ParticleEditor::EmittersUI(std::shared_ptr<UserInterface>(this), emUI.Size());
	emUI.Append(em);

	for (int i = 0; i < 4; i++)
	{
		col[i] = 1.f;
	}
	col[3] = 0.1f;
	
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

	//DrawGeneralSettings();

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
		ImGui::BeginDock("General Settings", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			
		}
		ImGui::EndDock();
		ImGui::BeginDock("Emitters", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			
			for (int i = 0; i < emUI.Size(); i++)
			{
				emUI[i].DrawEmitter();
			}

			ImGui::Separator();
			ImGui::BeginChild("##c");
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 30);
				if (ImGui::ImageButton((void*)newEmittIcon->GetHandle(), ImVec2(20, 20)))
				{
					AddNewEmitter();
				}		
			}
			ImGui::EndChild();
			
		}
		ImGui::EndDock();
		ImGui::BeginDock("Render", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{

		}
		ImGui::EndDock();
		ImGui::BeginDock("Emitter Settings", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{

		}
		ImGui::EndDock();
		ImGui::BeginDock("Texture", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			ImGui::SameLine(10);
			ImGui::Image((void*)gridTexture->GetHandle(), 
				ImVec2(ImGui::GetWindowContentRegionWidth()-5, ImGui::GetWindowContentRegionWidth() - 10),
				ImVec2(0, 0), ImVec2(1, 1), ImVec4(col[0], col[1], col[2], col[3]));
			ImGui::SameLine(10);
			ImGui::Image((void*)emitterTexture->GetHandle(), 
				ImVec2(ImGui::GetWindowContentRegionWidth() - 10, ImGui::GetWindowContentRegionWidth() - 10));

			if(ImGui::TreeNode("Texture Preview"))
			{
				ImGui::Text("Background Color:");
				ImGui::ColorEdit4("##backgroundCol", col);

				ImGui::TreePop();
			}

			DrawTextureSettings();
		}
		ImGui::EndDock();
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
	if (ImGui::CollapsingHeader("General Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
	}
	if (ImGui::CollapsingHeader("Emitters", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
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
