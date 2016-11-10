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

	Math::mat4 getViewMatrix() const { return viewMatrix; }

	void setViewMatrix(const Math::mat4& mat) { this->viewMatrix = mat; }

	Math::mat4 getProjectionMatrix() const { return projectionMatrix; }
	void setProjectionMatrix(const Math::mat4& mat) { this->projectionMatrix = mat; }

private:

	float windowWidth;
	float windowHeight;

	Math::mat4 viewMatrix;
	Math::mat4 projectionMatrix;

};

}