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

private:	
	bool shutdown;

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