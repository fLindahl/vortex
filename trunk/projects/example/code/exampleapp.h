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

	std::shared_ptr<Render::ModelInstance> modelInstance;
	std::shared_ptr<Render::ModelInstance> modelInstance1;
	std::shared_ptr<Physics::SurfaceCollider> physicsCollider;
	std::shared_ptr<Physics::SurfaceCollider> physicsCollider1;

    std::shared_ptr<Physics::RigidBody> rBody;
    std::shared_ptr<Physics::RigidBody> rBody1;


	BaseGameFeature::KeyHandler* keyhandler;

	Render::GraphicsProperty* gProperty;
	Render::GraphicsProperty* gProperty1;

	Physics::PhysicsHit hit;

	Display::Window* window;

	std::string consoleBuffer;
};
} // namespace Example