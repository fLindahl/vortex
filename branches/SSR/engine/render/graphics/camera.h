#pragma once
#include "foundation/math/matrix4.h"

/************************************************************************/
/* SINGLETON

Holds View and Projection Matrix

*/
/************************************************************************/

namespace Graphics
{

class Camera
{
public:
	Camera();
	~Camera() {}
	
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

	void SetFov(const float& newFov) { this->fov = newFov; }
	float GetFov() { return this->fov; }

	void SetNearZ(const float& newNearZ) { this->nearZ = newNearZ; }
	float GetNearZ() { return this->nearZ; }
	
	void setFarZ(const float& newfarZ) { this->farZ = newfarZ; }
	float GetFarZ() { return this->farZ; }

	float GetAspectRatio() { return this->aspectRatio; }

protected:
	float fov;
	float nearZ;
	float farZ;
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

class MainCamera : public Camera
{
private:
	MainCamera() {}
	
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
	void operator=(Camera const&) = delete;
};

}
