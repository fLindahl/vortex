#include "config.h"
#include "camera.h"
#include "render/server/renderdevice.h"

namespace Graphics
{

MainCamera::MainCamera() : 
	aspectRatio(0.0f)
{
	//windowWidth = GameHandler::getInstance()->getWindowWidth();
	//windowHeight = GameHandler::getInstance()->getWindowHeight();

	this->view = Math::mat4::identity();
	this->UpdateProjectionMatrix();
	this->setViewMatrix(Math::mat4::identity());

}

void MainCamera::LookAt(const Math::vec4& target, const Math::vec4& up)
{
	this->setViewMatrix(Math::mat4::lookatrh(cameraPos, target, up));
}

void MainCamera::setViewMatrix(const Math::mat4& mat)
{
	this->view = mat;
	this->invView = Math::mat4::inverse(mat);
	this->viewProjection = Math::mat4::multiply(this->view, this->projection);
	this->invViewProjection = Math::mat4::multiply(this->invView, this->invProjection);
}

void MainCamera::UpdateProjectionMatrix()
{
	this->aspectRatio = (float)Render::RenderDevice::Instance()->GetRenderResolution().x / (float)Render::RenderDevice::Instance()->GetRenderResolution().y;
	
	this->projection = Math::mat4::perspfovrh(-this->fov, this->aspectRatio, this->nearZ, this->farZ);
	this->invProjection = Math::mat4::inverse(this->projection);
	this->viewProjection = Math::mat4::multiply(this->view, this->projection);
	this->invViewProjection = Math::mat4::multiply(this->invView, this->invProjection);

	this->viewToTextureSpaceMatrix = Math::mat4::perspfovrh(-this->fov, this->aspectRatio, -this->nearZ, -this->farZ);
}

}