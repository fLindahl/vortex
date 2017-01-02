#include "config.h"
#include "camera.h"
#include "render/server/renderdevice.h"

namespace Graphics
{

MainCamera::MainCamera()
{
	//windowWidth = GameHandler::getInstance()->getWindowWidth();
	//windowHeight = GameHandler::getInstance()->getWindowHeight();

	viewMatrix = Math::mat4::identity();
	this->UpdateProjectionMatrix();

}

void MainCamera::LookAt(const Math::vec4& target, const Math::vec4& up)
{
	this->viewMatrix = Math::mat4::lookatrh(cameraPos, target, up);
}

void MainCamera::UpdateProjectionMatrix()
{
	this->projectionMatrix = Math::mat4::perspfovrh(fov, (float)Render::RenderDevice::Instance()->GetRenderResolution().x / (float)Render::RenderDevice::Instance()->GetRenderResolution().y, nearZ, farZ);
}

}