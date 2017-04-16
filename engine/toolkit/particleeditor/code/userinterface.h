#pragma once
#include <memory>
#include "commands.h"
#include "emittersUI.h"
#include "imgui_color_gradient.h"

namespace ParticleEditor{ class Application; }

struct EditorSettings
{
	float timeScale = 1;
	float col[4];

	bool collision = true;
	bool local = false;
};

class UserInterface
{
public:
	UserInterface(std::shared_ptr<ParticleEditor::Application> app);
	~UserInterface();

	void Run();
	void AddNewEmitter();
	void DuplicateEmitter(std::shared_ptr<ParticleEditor::EmittersUI> newEmitter);

	void RemoveEmitter(int id);

	void UpdateActiveEmitter(int id);

	void Tooltip(std::string text);

private:
	void ExecuteShortcuts();
	void ShowFileMenu();
	void DrawDocks();
	void ModalWindows();

	void DrawTextureSettings();
	void DrawGeneralSettings();
	void DrawEmitters();
	void DrawRender();
	void DrawEmitterSettings();

	bool openPopup;
	bool texturePopup;
	bool colorPicker;

	Ptr<Render::TextureResource> newEmittIcon;
	Ptr<Render::TextureResource> openIcon;

	Ptr<Render::TextureResource> emitterTexture;
	Ptr<Render::TextureResource> whiteTexture;
	Ptr<Render::TextureResource> gridTexture;

	Edit::CommandManager* commandManager;
	std::shared_ptr<ParticleEditor::Application> application;

	std::map<int, std::shared_ptr<ParticleEditor::EmittersUI>> emUI;

	int activeEmitter;

	int emitterCount;

	EditorSettings edSet;
};

