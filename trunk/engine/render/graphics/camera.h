#pragma once
#include "foundation/math/matrix4.h"

/************************************************************************/
/* SINGLETON

Holds View and Projection Matrix

*/
/************************************************************************/

namespace Graphics
{

class MainCamera
{
private:
	MainCamera();

public:
	static MainCamera* Instance()
	{
		static MainCamera instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	MainCamera(MainCamera const&) = delete;
	void operator=(MainCamera const&) = delete;

	Math::Matrix4 getViewMatrix() const { return viewMatrix; }

	static Math::Matrix4 LookAt(const Math::Vector4 &eye, const Math::Vector4 &target, const Math::Vector4 &up);

	void setViewMatrix(const Math::Matrix4& mat) { this->viewMatrix = mat; }

	Math::Matrix4 getProjectionMatrix() const { return projectionMatrix; }
	void setProjectionMatrix(const Math::Matrix4& mat) { this->projectionMatrix = mat; }

private:

	float windowWidth;
	float windowHeight;

	Math::Matrix4 viewMatrix;
	Math::Matrix4 projectionMatrix;

};

}