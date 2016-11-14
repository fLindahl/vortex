//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <foundation/math/shapes/quad.h>
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

	std::shared_ptr<Render::MeshResource> mesh;
	std::shared_ptr<Render::TextureResource> texture;
	std::shared_ptr<Render::ModelInstance> modelInstance;
	std::shared_ptr<Render::ModelInstance> modelInstance1;
	std::shared_ptr<Render::ShaderObject> shader;
	std::shared_ptr<Render::ShaderObject> shader1;

	BaseGameFeature::KeyHandler* keyhandler;

	Render::GraphicsProperty* gProperty;
	Render::GraphicsProperty* gProperty1;

	Display::Window* window;

	char* consoleBuffer;
};
} // namespace Example