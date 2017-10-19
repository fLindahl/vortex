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
#include "render/resources/cubemapnode.h"
#include "render/softwarerender/rasterizer.h"
#include "render/server/resourceserver.h"
#include "render/server/shaderserver.h"
#include "render/graphics/camera.h"
#include "application/basegamefeature/keyhandler.h"
#include "physics/basecollider.h"
#include <physics/convexhullcollider.h>
#include <physics/rigidbody.h>
#include <physics/physicsserver.h>
//#include <application/game/staticentity.h>
#include "userinterface.h"
#include "imgui_dock.h"
#include "undo.h"
#include "commands.h"
#include "userinterface.h"
#include "render/frame/lightcullingpass.h"
#include "application/game/particleentity.h"


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

	bool renderGeoProxies;

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
	//Physics::PhysicsHit hit;
	
	/// ImGui functions
	void RenderUI();
	
	Util::Array<Ptr<Game::ParticleEmitter>> particleList;

	BaseGameFeature::KeyHandler* keyhandler;
	
	Edit::CommandManager* commandManager;

	Display::Window* window;
};
} // namespace LevelEditor