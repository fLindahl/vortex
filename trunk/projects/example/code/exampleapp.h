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
#include "keyhandler.h"
#include "render/resources/lightnode.h"
#include "render/softwarerender/rasterizer.h"
#include "render/server/graphicsserver.h"
#include "render/server/resourceserver.h"
#include "render/server/shaderserver.h"
#include "render/graphics/camera.h"
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
	float64 mouseX = 0;
	float64 mouseY = 0;
	float64 oldx = 0;
	float64 oldy = 0;
	float64 lightX = 0;
	float64 lightY = 0;
	float64 lightZ = 0;
	float64 z = 0;

	shared_ptr<Render::MeshResource> mesh;
	shared_ptr<Render::TextureResource> texture;
	shared_ptr<Render::ModelInstance> modelInstance;
	shared_ptr<Render::ModelInstance> modelInstance1;
	shared_ptr<Render::ShaderObject> shader;
	shared_ptr<Render::ShaderObject> shader1;

	Render::GraphicsProperty* gProperty;
	Render::GraphicsProperty* gProperty1;

	KeyHandler::KeyHandler keyhandler;

	Display::Window* window;
};
} // namespace Example