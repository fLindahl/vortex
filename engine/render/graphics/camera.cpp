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
	projectionMatrix = Math::mat4::perspfovrh(80, 1.777777777777778f, 0.05f, 1000.0f);

}

void MainCamera::LookAt(const Math::vec4& target, const Math::vec4& up)
{
	this->viewMatrix = Math::mat4::lookatrh(cameraPos, target, up);
}


}