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

	void LookAt(const Math::vec4& target, const Math::vec4& up);

	Math::mat4 getProjectionMatrix() const { return projectionMatrix; }
	void setProjectionMatrix(const Math::mat4& mat) { this->projectionMatrix = mat; }

	Math::vec4 GetPosition() { return this->cameraPos; }
	void SetPosition(const Math::vec4& pos) { this->cameraPos = pos; }

private:

	Math::mat4 viewMatrix;
	Math::mat4 projectionMatrix;
	Math::vec4 cameraPos;
};

}