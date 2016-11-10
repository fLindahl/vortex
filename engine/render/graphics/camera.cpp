#include "config.h"
#include "camera.h"

namespace Graphics
{

MainCamera::MainCamera()
{
	//windowWidth = GameHandler::getInstance()->getWindowWidth();
	//windowHeight = GameHandler::getInstance()->getWindowHeight();


	//TODO: These should obviously not be hardcoded
	viewMatrix = Math::mat4::identity();
	projectionMatrix = Math::mat4::perspfovrh(80, 1.0f, 0.01f, 100000.0f);

}

}