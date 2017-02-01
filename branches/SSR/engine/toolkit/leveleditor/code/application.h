//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "render/properties/graphicsproperty.h"
#include "foundation/math/matrix4.h"
#include "render/resources/lightnode.h"
#include "render/softwarerender/rasterizer.h"
#include "render/server/resourceserver.h"
#include "render/server/shaderserver.h"
#include "render/graphics/camera.h"
#include "application/basegamefeature/keyhandler.h"
#include "fysik/basecollider.h"
#include <fysik/surfacecollider.h>
#include <fysik/rigidbody.h>
#include <fysik/physicsserver.h>
#include <application/game/staticentity.h>
#include "userinterface.h"
#include "imgui_dock.h"
#include "undo.h"
#include "commands.h"
#include "userinterface.h"
#include "application/game/modelentity.h"

namespace LevelEditor
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
	friend class Toolkit::UserInterface;

	bool shutdown = false;

	void CameraMovement();
	
	void DoPicking();

	Math::point cameraPos;
	float camRotX;
	float camRotY;

	Toolkit::UserInterface* UI;
	
	Math::point rayStart;
	Math::point rayEnd;
	Math::point reflectStart;
	Math::point reflectEnd;
	Physics::PhysicsHit hit;
	
	/// ImGui functions
	void RenderUI();

	// show some nanovg stuff
	void RenderNano(NVGcontext * vg);
	
	std::shared_ptr<Game::ModelEntity> sponza;

	std::shared_ptr<Game::StaticEntity> wall1;
	std::shared_ptr<Game::StaticEntity> wall2;
	std::shared_ptr<Game::StaticEntity> wall3;
	std::shared_ptr<Game::StaticEntity> wall4;
	std::shared_ptr<Game::StaticEntity> floor;
	std::shared_ptr<Game::StaticEntity> ceiling;
	
	BaseGameFeature::KeyHandler* keyhandler;
	
	Edit::CommandManager* commandManager;

	Display::Window* window;

	GLfloat* depthPixels;
	GLfloat* normalPixels;
};
} // namespace LevelEditor