//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "application/basegamefeature/keyhandler.h"
#include "core/refcounted.h"

namespace Example
{
	class TestClass : public Core::RefCounted
	{
	__DeclareClass(TestClass)
	public:
		TestClass();
		~TestClass();

		int value;

	private:
		
	};

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

	/// Resize window callback
	void ResizeWindow(int32, int32);

	BaseGameFeature::KeyHandler* keyhandler;
	
	Display::Window* window;
};
} // namespace Example