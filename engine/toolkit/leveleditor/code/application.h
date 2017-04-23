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
	Physics::PhysicsHit hit;
	
	/// ImGui functions
	void RenderUI();

	Ptr<Render::ModelInstance> modelInstance;
	Ptr<Render::ModelInstance> modelInstanceScene;
	
	Ptr<Game::RigidBodyEntity> rigidBodyEntity1;
	Ptr<Game::RigidBodyEntity> rigidBodyEntity2;
	Ptr<Game::RigidBodyEntity> rigidBodyEntity3;
	Ptr<Game::RigidBodyEntity> rigidBodyEntity4;
	Ptr<Game::RigidBodyEntity> rigidBodyEntity5;

	Ptr<Game::StaticEntity> SceneEntity1;
    Ptr<Game::StaticEntity> SceneEntity2;
    Ptr<Game::StaticEntity> SceneEntity3;
    Ptr<Game::StaticEntity> SceneEntity4;
    Ptr<Game::StaticEntity> SceneEntity5;
    Ptr<Game::StaticEntity> SceneEntity6;
	Ptr<Game::StaticEntity> SceneEntity7;

	Ptr<Game::ModelEntity> sponza;
	Ptr<Game::ParticleEntity> billboard;
	Ptr<Game::ParticleEntity> billboard2;

	Ptr<Game::ModelEntity> wall1;
	Ptr<Game::ModelEntity> wall2;
	Ptr<Game::ModelEntity> wall3;
	Ptr<Game::ModelEntity> wall4;
	Ptr<Game::ModelEntity> floor;
	Ptr<Game::ModelEntity> ceiling;
	
	Util::Array<Ptr<Game::ParticleEntity>> particleList;

	//Particles::ParticleSettings pSettings;

	BaseGameFeature::KeyHandler* keyhandler;
	
	Edit::CommandManager* commandManager;

	Display::Window* window;

	GLuint* pickingPixels;

    //Render::LightCullingPass ls;
};
} // namespace LevelEditor