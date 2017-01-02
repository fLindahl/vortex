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
	bool shutdown = false;

	bool buttonAlreadyPressed = false;

	Math::point cameraPos;
	float camRotX;
	float camRotY;
	void CameraMovement();

	Math::vec4 rayStart;
	Math::vec4 rayEnd;
	Physics::PhysicsHit hit;

	void DoPicking();

	/// ImGui functions
	void RenderUI();
	void RenderDocks();
	void ExecShortCuts();
	void ShowFileMenu();
	/// ImGui Variables

	// show some nanovg stuff
	void RenderNano(NVGcontext * vg);

	double frameTime;
	double prevFPSTime;
	double currentFPS;

	std::shared_ptr<Render::ModelInstance> modelInstance;
	std::shared_ptr<Render::ModelInstance> modelInstance1;
	std::shared_ptr<Render::ModelInstance> modelInstanceScene;
	
	std::shared_ptr<Game::RigidBodyEntity> rigidBodyEntity1;
	std::shared_ptr<Game::RigidBodyEntity> rigidBodyEntity2;
	std::shared_ptr<Game::RigidBodyEntity> rigidBodyEntity3;
	std::shared_ptr<Game::RigidBodyEntity> rigidBodyEntity4;
	std::shared_ptr<Game::RigidBodyEntity> rigidBodyEntity5;
    Util::Array<std::shared_ptr<Game::RigidBodyEntity>> RBEs;

	std::shared_ptr<Game::StaticEntity> SceneEntity1;
    std::shared_ptr<Game::StaticEntity> SceneEntity2;
    std::shared_ptr<Game::StaticEntity> SceneEntity3;
    std::shared_ptr<Game::StaticEntity> SceneEntity4;
    std::shared_ptr<Game::StaticEntity> SceneEntity5;
    std::shared_ptr<Game::StaticEntity> SceneEntity6;

	BaseGameFeature::KeyHandler* keyhandler;
	
	Edit::CommandManager commandManager;

	Display::Window* window;

	std::string consoleBuffer;
	std::string a;
};
} // namespace LevelEditor