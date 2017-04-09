#pragma once
#include <memory>
#include "commands.h"
#include "emittersUI.h"

namespace ParticleEditor{ class Application; }

struct EditorSettings
{
	float timeScale = 1;
	float col[4];
};

class UserInterface
{
public:
	UserInterface(std::shared_ptr<ParticleEditor::Application> app);
	~UserInterface();

	void Run();
	void AddNewEmitter();
	void DuplicateEmitter(ParticleEditor::EmittersUI newEmitter);

	void UpdateActiveEmitter(int id);

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

	Ptr<Render::TextureResource> newEmittIcon;

	Ptr<Render::TextureResource> emitterTexture;
	Ptr<Render::TextureResource>gridTexture;

	Edit::CommandManager* commandManager;
	std::shared_ptr<ParticleEditor::Application> application;

	Util::Array<ParticleEditor::EmittersUI> emUI;

	int activeEmitter;

	EditorSettings edSet;
};

