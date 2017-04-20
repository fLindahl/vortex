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
	emitterCount = 0;

	openPopup = false;
	saveAsPopup = false;
	saveAsCheck = true;

	isSaved = false;

	texturePopup = false;
	colorPicker = false;

	filename = "";

	newEmittIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/add.png");
	openIcon = Render::ResourceServer::Instance()->LoadTexture("engine/toolkit/particleeditor/resources/textures/open.png");

	emitterTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/particles/particle_spritesheet_smoke2.tga");
	whiteTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/whiteBackground.tga");
	gridTexture = Render::ResourceServer::Instance()->LoadTexture("resources/textures/grid.png");

	std::shared_ptr<ParticleEditor::EmittersUI> em = std::make_shared<ParticleEditor::EmittersUI>(this, emitterCount, true);
	em->ev.active = true;
	emUI.Append(em);
	emitterCount++;

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
	ExecuteShortcuts();
	ModalWindows();
}


void UserInterface::ExecuteShortcuts()
{
	if ((ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_O))))
	{
		this->openPopup = true;
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_S)))
	{
		this->saveAsCheck = false;
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_S)))
	{
		this->saveAsPopup = true;
	}
	else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_::ImGuiKey_N)))
	{
		
	}
}

void UserInterface::ShowFileMenu()
{
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		openPopup = true;
	}
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		this->saveAsCheck = false;
	}
	if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
	{
		this->saveAsPopup = true;
	}

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
			ImGui::Image((void*)whiteTexture->GetHandle(),
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
	if (!this->saveAsCheck)
	{
		if (this->isSaved)
		{
			Particles::ParticleFile::Instance()->SaveParticle(filename, emUI);
			this->saveAsCheck = true;
		}
		else
		{
			this->saveAsPopup = true;
		}
	}
	if (this->saveAsPopup){ ImGui::OpenPopup("SaveAsFile"); }
	if (this->texturePopup){ ImGui::OpenPopup("OpenTexture"); }

	if (ImGui::BeginPopupModal("OpenFile", &this->openPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("particle", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);
			
			for (int i = emUI.Size() - 1; i >= 1; i--)
				RemoveEmitter(i);

			IO::Console::Instance()->Print("NFD opened succesfully", IO::INPUT);

			Util::String s = outpath;
			Util::Array<Util::String> path;

			s.ConvertBackslashes();
			filename = s.ExtractFileName();
			Util::String title = "Particle Editor - " + s.ExtractFileName();
			application->GetWindow()->SetTitle(title.AsCharPtr());

			LoadSettings(Particles::ParticleFile::Instance()->LoadParticle(s));

			this->isSaved = true;
			this->openPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			IO::Console::Instance()->Print("User canceled nfd", IO::INPUT);
			this->openPopup = false;
		}
		else
		{
			IO::Console::Instance()->Print("nfd failed to open", IO::ERROR);
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->openPopup = false;
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("SaveAsFile", &this->saveAsPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_SaveDialog("particle", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);
			IO::Console::Instance()->Print("NFD opened succesfully", IO::INPUT);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			
			s.ConvertBackslashes();
			
			filename = s.ExtractFileName();
			Util::String title = "Particle Editor - " + s.ExtractFileName();
			application->GetWindow()->SetTitle(title.AsCharPtr());

			Particles::ParticleFile::Instance()->SaveParticle(filename, emUI);

			this->isSaved = true;
			this->saveAsCheck = true;
			this->saveAsPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			IO::Console::Instance()->Print("User canceled nfd", IO::INPUT);
			this->saveAsCheck = true;
			this->saveAsPopup = false;
		}
		else
		{
			IO::Console::Instance()->Print("nfd failed to open", IO::ERROR);
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->saveAsCheck = true;
			this->openPopup = false;
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("OpenTexture", &this->texturePopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("tga,png;jpg,jpeg", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			s.ConvertBackslashes();
			emUI[activeEmitter]->settings.texName = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr();
			
			emitterTexture = Render::ResourceServer::Instance()->LoadTexture(outpath);

			this->texturePopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->texturePopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->texturePopup = false;
		}

		ImGui::EndPopup();
	}
	
}

void UserInterface::LoadSettings(Util::Array<Particles::FileSettings> file)
{
	for (auto itr : file)
	{
		AddNewEmitter(itr);
	}
}

void UserInterface::DrawTextureSettings()
{
	ImGui::BeginChild("##backgroundtex", ImVec2(ImGui::GetWindowContentRegionWidth(), 150), true);
	{
		ImGui::Text("SpriteSheet"); ImGui::SameLine(140);
		if (ImGui::Checkbox("##texsheet", &emUI[activeEmitter]->settings.spriteSheetTex))
		{
			//emitter->GetRenderBuffer().textureAnimation[3] = int(emitter->GetParticleUISettings().spriteSheetTex);
			//emitter->UpdateUniformBuffer();
		}
		ImGui::Text("Frames per row"); ImGui::SameLine(140);
		ImGui::PushItemWidth(80);
		if (ImGui::InputInt("##fpr1", &emUI[activeEmitter]->settings.framesPerRow, 0, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//emitter->GetRenderBuffer().textureAnimation[0] = emitter->GetParticleUISettings().framesPerRow;
			//emitter->UpdateUniformBuffer();
		}
		ImGui::PopItemWidth();

		ImGui::Text("Total frames"); ImGui::SameLine(140);
		ImGui::PushItemWidth(80);
		if (ImGui::InputInt("##tnf2", &emUI[activeEmitter]->settings.numberOfFrames, 0, 255, ImGuiInputTextFlags_EnterReturnsTrue))
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
		const char* pTypeItems[] = { "Continuous", "One shot" };
		static int pTypeItemsPos = 0;
		ImGui::Text("Type:");
		ImGui::SameLine(160);
		ImGui::Combo("##pTypeCombo", &pTypeItemsPos, pTypeItems, IM_ARRAYSIZE(pTypeItems));

		ImGui::Text("Time Scale:");
		ImGui::SameLine(160);
		ImGui::DragFloat("##TimeScale", &edSet.timeScale, 0.01f, 0.01f, 10.f);
	}
	ImGui::EndChild();
}

void UserInterface::DrawEmitters()
{
	ImGui::BeginChild("##es", ImVec2(ImGui::GetWindowWidth() - 20, 45 * (emUI.Size() <= 8 ? emUI.Size()-1 : 8)+5), false);
	{
		for (int i = 0; i < emUI.Size(); i++)
		{
			emUI[i]->DrawEmitter();
		}
	}
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginChild("##cem", ImVec2(ImGui::GetContentRegionAvailWidth(), 40));
	{
		ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 40);
		if (ImGui::ImageButton((void*)newEmittIcon->GetHandle(), ImVec2(20, 20)))
		{
			AddNewEmitter();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			{
				ImGui::Text("Create new emitter");
			}
			ImGui::EndTooltip();
		}
	}
	ImGui::EndChild();
}

void UserInterface::DrawRender()
{
	ImGui::SameLine(15);
	ImGui::BeginChild("##render", ImVec2(ImGui::GetWindowWidth() - 30, ImGui::GetWindowHeight() - 30), true);
	{
		const char* pMeshItems[] = { "Billboard", "Mesh" };
		static int pMeshItemsPos = 0;
		ImGui::Text("Particle:"); 
		ImGui::SameLine(160); 
		ImGui::Combo("##pMeshCombo", &pMeshItemsPos, pMeshItems, IM_ARRAYSIZE(pMeshItems));

		ImGui::Text("Texture:");
		ImGui::SameLine(160);
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 100);

		if (emUI[activeEmitter]->settings.texName != "")
		{
			Util::String s = emUI[activeEmitter]->settings.texName.AsCharPtr();
			Util::Array<Util::String> path;

			s.Tokenize("/", path);

			s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

			ImGui::InputText("##texture", (char*) s.AsCharPtr(), 256, ImGuiInputTextFlags_ReadOnly);
		}
		else
		{
			ImGui::InputText("##texture", (char*)emUI[activeEmitter]->settings.texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
		}
		
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if(ImGui::ImageButton((void*) openIcon->GetHandle(), ImVec2(18.f,15.f)))
		{
			this->texturePopup = true;
		}

		const char* pBlendItems[] = { "Additive", "Alpha", "Multiply" };
		static int pBlendItemsPos = 0;
		ImGui::Text("Blend type:");
		ImGui::SameLine(160);
		ImGui::Combo("##pBlendCombo", &pBlendItemsPos, pBlendItems, IM_ARRAYSIZE(pBlendItems));

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		const char* pColorItems[] = { "Solid", "Fade" };
		static int pColorItemsPos = 0;
		ImGui::Text("Color Setting:");
		ImGui::SameLine(160);
		ImGui::Combo("##pColorCombo", &pColorItemsPos, pColorItems, IM_ARRAYSIZE(pColorItems));

		if (pColorItemsPos == 0)
		{			
			ImGui::Text("Color:");
			ImGui::SameLine(160);
			if (ImGui::ColorButton(ImVec4(emUI[activeEmitter]->settings.color[0], emUI[activeEmitter]->settings.color[1], emUI[activeEmitter]->settings.color[2], emUI[activeEmitter]->settings.color[3]), ImVec2(415, 0)))
			{
				colorPicker = true;
			}

			if (colorPicker)
			{
				ImGui::Begin("Color Picker", &this->colorPicker, ImGuiWindowFlags_ShowBorders);
				{
					ImGui::ColorPicker4("##SolidColorPick", (float*)&emUI[activeEmitter]->settings.color, ImGuiColorEditFlags_Alpha);
				}
				ImGui::End();
			}
		}
		else if (pColorItemsPos == 1)
		{
			ImGui::Text("Color:");
			ImGui::SameLine(160);
			if (ImGui::GradientButton(&emUI[activeEmitter]->gradient, gridTexture->GetHandle()))
			{
				colorPicker = true;
			}

			if (colorPicker)
			{
				ImGui::Begin("Color Picker", &this->colorPicker, ImGuiWindowFlags_ShowBorders);
				{
					ImGui::GradientEditor(&emUI[activeEmitter]->gradient, emUI[activeEmitter]->draggingMark, emUI[activeEmitter]->selectedMark, gridTexture->GetHandle());
				}
				ImGui::End();
			}
					
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Size over time:");
		ImGui::SameLine(160);
		ImGui::Checkbox("##sizeovertime", &emUI[activeEmitter]->settings.sizeOverTime);
		
		if (emUI[activeEmitter]->settings.sizeOverTime)
		{
			ImGui::Text("Size:");
			ImGui::SameLine(160);
			ImGui::DragFloat2("##endSize", emUI[activeEmitter]->settings.size, 0.01f, 0.01f, 10.f);
			Tooltip("Set start and end size of the particle");
		}
		else
		{
			ImGui::Text("Size:");
			ImGui::SameLine(160);
			ImGui::DragFloat("##startSize", &emUI[activeEmitter]->settings.size[0], 0.01f, 0.01f, 10.f);
			Tooltip("Set the size of the particle");
		}

		ImGui::Text("Max Particles:");
		ImGui::SameLine(160);
		ImGui::InputInt("##maxParticles", &emUI[activeEmitter]->settings.numParticles, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
		

	}
	ImGui::EndChild();
}

void UserInterface::DrawEmitterSettings()
{
	ImGui::SameLine(15);
	ImGui::BeginChild("##emitterSettings", ImVec2(ImGui::GetWindowWidth() - 30, ImGui::GetWindowHeight() - 40), true);
	{
		ImGui::NewLine();

		ImGui::Text("Local:");
		ImGui::SameLine(160);
		ImGui::Checkbox("##locEmitt", &edSet.local);

		ImGui::Text("Collision:");
		ImGui::SameLine(160);
		ImGui::Checkbox("##collision", &edSet.collision);

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		const char* items[] = { "Cone", "Sphere", "Hemisphere" };
		static int item2 = 0;
		ImGui::Text("Emitter Shape:"); 
		ImGui::SameLine(160);
		if (ImGui::Combo("##cbs", &item2, items, IM_ARRAYSIZE(items)))
		{
			emUI[activeEmitter]->settings.shapes = (Particles::EmitterShapes)item2;
		}

		if (emUI[activeEmitter]->settings.shapes == Particles::CONE)
		{
			ImGui::Text("Cone Radius:"); 
			ImGui::SameLine(160);
			ImGui::SliderFloat("##cRadius", &emUI[activeEmitter]->settings.radius, 0.001, 10, "%.3f", 5);
		}
		else if (emUI[activeEmitter]->settings.shapes == Particles::SPHERE)
		{
			ImGui::Text("Sphere Radius:"); 
			ImGui::SameLine(160);
			ImGui::SliderFloat("##sRadius", &emUI[activeEmitter]->settings.radius, 0.001, 10, "%.3f", 5);
		}
		else if (emUI[activeEmitter]->settings.shapes == Particles::HEMISPHERE)
		{
			ImGui::Text("Hemisphere Radius:"); 
			ImGui::SameLine(160);
			ImGui::SliderFloat("##hsRadius", &emUI[activeEmitter]->settings.radius, 0.001, 10, "%.3f", 5);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Random lifetime:");
		ImGui::SameLine(160);
		ImGui::Checkbox("##lifeRand", &emUI[activeEmitter]->settings.lifeTimeRand);

		if (!emUI[activeEmitter]->settings.lifeTimeRand)
		{
			ImGui::Text("Lifetime:");
			ImGui::SameLine(160);
			ImGui::SliderFloat("##lifeMin", &emUI[activeEmitter]->settings.acc[3], 0.001, 100, "%.3f", 5);
		}
		else
		{
			ImGui::Text("Lifetime Min:");
			ImGui::SameLine(160);
			ImGui::SliderFloat("##lifeMin", &emUI[activeEmitter]->settings.acc[3], 0.001, 100, "%.3f", 5);

			ImGui::Text("Lifetime Max:");
			ImGui::SameLine(160);
			ImGui::SliderFloat("##lifeMax", &emUI[activeEmitter]->settings.acc2[3], 0.001, 100, "%.3f", 5);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Random Velocity:");
		ImGui::SameLine(160);
		ImGui::Checkbox("##velRand", &emUI[activeEmitter]->settings.velRand);

		if (!emUI[activeEmitter]->settings.velRand)
		{
			ImGui::Text("Velocity:");
			ImGui::SameLine(160);
			ImGui::DragFloat("##velMin", &emUI[activeEmitter]->settings.vel, 0.01, 0.001, 1000, "%.3f");
		}
		else
		{
			ImGui::Text("Velocity Min:");
			ImGui::SameLine(160);
			ImGui::DragFloat("##velMin", &emUI[activeEmitter]->settings.vel, 0.01, 0.001, 1000, "%.3f");

			ImGui::Text("Velocity Max:");
			ImGui::SameLine(160);
			ImGui::DragFloat("##velMax", &emUI[activeEmitter]->settings.vel2, 0.01, 0.001, 1000, "%.3f");
		}

		ImGui::NewLine();

		ImGui::Text("Random Acceleration:");
		ImGui::SameLine(160);
		ImGui::Checkbox("##accRand", &emUI[activeEmitter]->settings.accRand);

		if (!emUI[activeEmitter]->settings.accRand)
		{
			ImGui::Text("Acceleration:");
			ImGui::SameLine(160);
			ImGui::DragFloat3("##accMin", (float*)&emUI[activeEmitter]->settings.acc, 0.01f, -50.f, 50.f, "%.3f", 5);
		}
		else
		{
			ImGui::Text("Acceleration Min:");
			ImGui::SameLine(160);
			ImGui::DragFloat3("##accMin", (float*)&emUI[activeEmitter]->settings.acc, 0.01f, -50, 50, "%.3f", 5);

			ImGui::Text("Acceleration Max:");
			ImGui::SameLine(160);
			ImGui::DragFloat3("##accMax", (float*)&emUI[activeEmitter]->settings.acc2, 0.01f, -50.f, 50.f, "%.3f", 5);
		}

	}
	ImGui::EndChild();
}

void UserInterface::AddNewEmitter()
{
	std::shared_ptr<ParticleEditor::EmittersUI> tempEm = std::make_shared<ParticleEditor::EmittersUI>(this, emitterCount);
	emUI.Append(tempEm);
	emitterCount++;
}

void UserInterface::AddNewEmitter(Particles::FileSettings set)
{
	std::shared_ptr<ParticleEditor::EmittersUI> tempEm = std::make_shared<ParticleEditor::EmittersUI>(this, emitterCount);
	tempEm->settings = set.set;
	tempEm->settings.texName = set.texPath.AsCharPtr();
	tempEm->ev.name = set.name.AsCharPtr();
	emUI.Append(tempEm);
	emitterCount++;
}

void UserInterface::DuplicateEmitter(std::shared_ptr<ParticleEditor::EmittersUI> newEmitter)
{
	//TODO: Fix the problem with pointer deletion when things are copied
	std::shared_ptr<ParticleEditor::EmittersUI> tempEm = std::make_shared<ParticleEditor::EmittersUI>(this, emitterCount);
	tempEm->ev = newEmitter->ev;
	tempEm->settings = newEmitter->settings;
	tempEm->ev.active = false;
	emUI.Append(tempEm);
	emitterCount++;
}

void UserInterface::RemoveEmitter(int id)
{
	if (emUI.Size() > 1)
	{
		emUI.RemoveIndex(id);
		
		for (int i = id; i < emUI.Size(); i++)
		{
			emUI[i]->id = emUI.FindIndex(emUI[i]);			
		}

		emitterCount--;
	}
	
}

void UserInterface::UpdateActiveEmitter(int id)
{
	for (int i = 0; i < emUI.Size(); i++)
	{
		if (i != id)
			emUI[i]->ev.active = false;
	}

	activeEmitter = id;

	if (emUI[activeEmitter]->settings.texName != "")
	{
		Util::String s = emUI[activeEmitter]->settings.texName.AsCharPtr();
		emitterTexture = Render::ResourceServer::Instance()->LoadTexture(s);
	}

	//emUI[id] use to set the values in the editor
}

void UserInterface::Tooltip(std::string text)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		{
			ImGui::Text(text.c_str());
		}
		ImGui::EndTooltip();
	}
}
