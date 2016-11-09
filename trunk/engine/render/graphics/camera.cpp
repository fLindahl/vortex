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

Math::Matrix4 MainCamera::LookAt( const Math::Vector4& eye, const Math::Vector4& target, const Math::Vector4& up )
{
	Math::Vector4 zaxis = (eye - target).normalized();    // The "forward" vector.
	Math::Vector4 xaxis = Math::Vector4::cross(up, zaxis).normalized();// The "right" vector.
	Math::Vector4 yaxis = Math::Vector4::cross(zaxis, xaxis);     // The "up" vector.

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	Math::Matrix4 viewMatrix = {
		xaxis.x(), yaxis.x(), zaxis.x(), 0.0f,
		xaxis.y(), yaxis.y(), zaxis.y(), 0.0f,
		xaxis.z(), yaxis.z(), zaxis.z(), 0.0f,
		-Math::Vector4::dot(xaxis, eye), -Math::Vector4::dot(yaxis, eye), -Math::Vector4::dot(zaxis, eye), 1
	};

	return viewMatrix;
}

}