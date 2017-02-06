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

	Math::mat4 getView() const { return view; }
	Math::mat4 getInvView() const { return invView; }

	void setViewMatrix(const Math::mat4& mat);

	void LookAt(const Math::vec4& target, const Math::vec4& up);

	Math::mat4 getProjection() const { return projection; }
	Math::mat4 getInvProjection() const { return invProjection; }

	void UpdateProjectionMatrix();

	Math::mat4 getViewProjection() const { return viewProjection; }
	Math::mat4 getInvViewProjection() const { return invViewProjection; }

	Math::mat4 getViewToTextureSpace() const { return viewToTextureSpaceMatrix; }

	Math::vec4 GetPosition() const;
	void SetPosition(const Math::point& pos);

private:

	const float fov = 1.5708f;
	const float nearZ = 0.05f;
	const float farZ = 1000.0f;
	float aspectRatio;

	Math::mat4 view;
	Math::mat4 invView;
	Math::mat4 projection;
	Math::mat4 invProjection;
	Math::mat4 viewProjection;
	Math::mat4 invViewProjection;

	//For converting viewspace coordinates to screen pixel coordinates.
	Math::mat4 viewToTextureSpaceMatrix;
};

}
