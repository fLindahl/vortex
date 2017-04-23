//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "application/basegamefeature/keyhandler.h"
#include "imgui_dock.h"
#include "undo.h"
#include "commands.h"
#include "userinterface.h"
#include "application/game/particleentity.h"

namespace ParticleEditor
{
class Application : public Core::App
{
public:
	/// constructor
	Application();
	/// destructor
	~Application();

	/// open app
	bool Open();
	/// run app
	void Run();

	void Shutdown(bool val){ shutdown = val; };

	Display::Window* GetWindow(){ return window; };

private:	
	bool shutdown;

	std::shared_ptr<UserInterface> UI;

	std::shared_ptr<Game::ParticleEntity> billboard;

	Util::Array<Ptr<Game::ParticleEmitter>> particleList;

	/// ImGui functions
	void RenderUI();

	// show some nanovg stuff
	void RenderNano(NVGcontext * vg);
	
	//Input handler
	BaseGameFeature::KeyHandler* keyhandler;
	
	//Undo and Redo queue
	Edit::CommandManager* commandManager;

	//GL context
	Display::Window* window;
};
} // namespace LevelEditor