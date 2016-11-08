#include "config.h"
#include "camera.h"

namespace Graphics
{

MainCamera::MainCamera()
{
	//windowWidth = GameHandler::getInstance()->getWindowWidth();
	//windowHeight = GameHandler::getInstance()->getWindowHeight();


	//TODO: These should obviously not be hardcoded
	viewMatrix = Math::Matrix4::identity();
	projectionMatrix = Math::Matrix4::perspectiveMatrix(100000.0f, 0.01f, 1.0f, 75);

}

}