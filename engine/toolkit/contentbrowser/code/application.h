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
#include "render/softwarerender/rasterizer.h"
#include "render/server/resourceserver.h"
#include "render/server/shaderserver.h"
#include "render/graphics/camera.h"
#include "application/basegamefeature/keyhandler.h"
#include "physics/basecollider.h"
#include <physics/surfacecollider.h>
#include <physics/rigidbody.h>
#include <physics/physicsserver.h>
#include "userinterface.h"
#include "imgui_dock.h"
#include "userinterface.h"

namespace ContentBrowser
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

	void NewModel();
	void SaveModel(const char* filepath);
	void ExportMesh(const char* filepath);

	Math::point cameraPos;
	float camRotX;
	float camRotY;

	Toolkit::UserInterface* UI;
	
	/// ImGui functions
	void RenderUI();

	//Ptr<Game::ModelEntity> loadedModel;
    
	BaseGameFeature::KeyHandler* keyhandler;

	Display::Window* window;
};
} // namespace LevelEditor