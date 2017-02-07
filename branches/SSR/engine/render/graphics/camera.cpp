#include "config.h"
#include "camera.h"
#include "render/server/renderdevice.h"

namespace Graphics
{

Camera::Camera() : 
	aspectRatio(0.0f),
	fov(1.5708f),
	nearZ(0.05f),
	farZ(1000.0f)
{
	//windowWidth = GameHandler::getInstance()->getWindowWidth();
	//windowHeight = GameHandler::getInstance()->getWindowHeight();

	this->view = Math::mat4::identity();
	this->UpdateProjectionMatrix();
	this->setViewMatrix(Math::mat4::identity());
}

void Camera::LookAt(const Math::vec4& target, const Math::vec4& up)
{
	this->setViewMatrix(Math::mat4::lookatrh(view.get_position(), target, up));
}

void Camera::setViewMatrix(const Math::mat4& mat)
{
	this->view = mat;
	this->invView = Math::mat4::inverse(mat);
	this->viewProjection = Math::mat4::multiply(this->view, this->projection);
	this->invViewProjection = Math::mat4::multiply(this->invView, this->invProjection);
}

void Camera::UpdateProjectionMatrix()
{
	Render::Resolution res = Render::RenderDevice::Instance()->GetRenderResolution();
	this->aspectRatio = (float)res.x / (float)res.y;
	
	this->projection = Math::mat4::perspfovrh(-this->fov, this->aspectRatio, this->nearZ, this->farZ);
	this->invProjection = Math::mat4::inverse(this->projection);
	this->viewProjection = Math::mat4::multiply(this->view, this->projection);
	this->invViewProjection = Math::mat4::multiply(this->invView, this->invProjection);

	float sx = (float)res.x / 2.0f;
	float sy = (float)res.y / 2.0f;
	
	Math::mat4  scrScale = Math::mat4(sx, 0.0f, 0.0f, 0.0f,
									  0.0f, sy, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f,
									  sx, sy, 0.0f, 1.0f);

	this->viewToTextureSpaceMatrix = Math::mat4::multiply(this->projection, scrScale);
}

Math::vec4 Camera::GetPosition() const
{
	return this->invView.get_position();
}

void Camera::SetPosition(const Math::point& p)
{
	this->view.set_position(p);
	this->invView = Math::mat4::inverse(view);
	this->viewProjection = Math::mat4::multiply(this->view, this->projection);
	this->invViewProjection = Math::mat4::multiply(this->invView, this->invProjection);
}

}