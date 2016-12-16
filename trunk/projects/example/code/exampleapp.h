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
namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	/// show some ui things
	void RenderUI();

	// show some nanovg stuff
	void RenderNano(NVGcontext * vg);


	double frameTime;

	std::shared_ptr<Render::ModelInstance> modelInstance;
	std::shared_ptr<Render::ModelInstance> modelInstance1;
	std::shared_ptr<Render::ModelInstance> modelInstanceScene;

	std::shared_ptr<Physics::SurfaceCollider> physicsCollider;
	std::shared_ptr<Physics::SurfaceCollider> physicsCollider1;
	std::shared_ptr<Physics::SurfaceCollider> SceneCollider;

    std::shared_ptr<Physics::RigidBody> rBody1;
    std::shared_ptr<Physics::RigidBody> rBody2;
    std::shared_ptr<Physics::RigidBody> rBody3;
    std::shared_ptr<Physics::RigidBody> rBody4;
    std::shared_ptr<Physics::RigidBody> rBody5;
    Util::Array<std::shared_ptr<Physics::RigidBody>> RBs;

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

	Render::GraphicsProperty* gProperty;
	Render::GraphicsProperty* gProperty1;
	Render::GraphicsProperty* gProperty2;
	Render::GraphicsProperty* gProperty3;
	Render::GraphicsProperty* gProperty4;
    Util::Array<Render::GraphicsProperty*> gPs;

	Render::GraphicsProperty* gPropertyScene1;
    Render::GraphicsProperty* gPropertyScene2;
    Render::GraphicsProperty* gPropertyScene3;
    Render::GraphicsProperty* gPropertyScene4;
    Render::GraphicsProperty* gPropertyScene5;
    Render::GraphicsProperty* gPropertyScene6;


	Physics::PhysicsHit hit;

	Display::Window* window;

	std::string consoleBuffer;
	std::string a;
};
} // namespace Example