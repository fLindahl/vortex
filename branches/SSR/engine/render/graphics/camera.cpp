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
	Render::Resolution res = Render::RenderDevice::Instance()->GetRenderResolution();
	this->aspectRatio = (float)res.x / (float)res.y;
	
	this->projection = Math::mat4::perspfovrh(-this->fov, this->aspectRatio, this->nearZ, this->farZ);
	this->invProjection = Math::mat4::inverse(this->projection);
	this->viewProjection = Math::mat4::multiply(this->view, this->projection);
	this->invViewProjection = Math::mat4::multiply(this->invView, this->invProjection);

	float sx = (float)res.x / 2.0f;
	float sy = (float)res.y / 2.0f;

	float xOffset = (float)res.x * 0.5f;
	float yOffset = (float)res.y * 0.5f;

	Math::mat4  scrScale = Math::mat4(sx, 0.0f, 0.0f, 0.0f,
									  0.0f, sy, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f,
									  sx, sy, 0.0f, 1.0f);

	this->viewToTextureSpaceMatrix = scrScale;
}

}