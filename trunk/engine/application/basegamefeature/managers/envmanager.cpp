#include "config.h"
#include "envmanager.h"
#include "render/server/renderdevice.h"

namespace BaseGameFeature
{

Math::line EnvManager::ComputeMouseWorldRay(const double& cursorX, const double& cursorY, const float& length, const float& viewPortSizeX, const float& viewPortSizeY)
{
	//mouse_pos_in_dock.x = Math::min(mouse_pos_in_dock.x, mouse_pos_in_dock.y);

	Render::Resolution res = Render::RenderDevice::Instance()->GetRenderResolution();

	Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();
	Math::mat4 invProj = Graphics::MainCamera::Instance()->getInvProjection();
	//Math::mat4 invViewProj = Graphics::MainCamera::Instance()->getInvViewProjection();
	//Math::mat4 viewProj = Graphics::MainCamera::Instance()->getViewProjection();

	double cursorPosX = 0.0f;
	double cursorPosY = 0.0f;

	// Transform to world coordinates
	cursorPosX = (((cursorX / viewPortSizeX) - 0.5f) * 2.0f);
	cursorPosY = (((cursorY / viewPortSizeY) - 0.5f) * 2.0f);
	Math::vec4 cursorTransform = Math::vec4((float)cursorPosX, (float)cursorPosY, 1.0, 1.0f);

	printf("cursorpos screenspace : %f, %f, %f, %f\n", cursorTransform.x(), cursorTransform.y(), cursorTransform.z(), cursorTransform.w());

	cursorTransform = Math::mat4::transform(cursorTransform, invProj);
	Math::point ray = (cursorTransform * 0.01f);
	Math::vec4 rayWorldPos = Math::mat4::transform(ray, invView);

	printf("rayWorldPos: %f %f %f %f\n", rayWorldPos.x(), rayWorldPos.y(), rayWorldPos.z(), rayWorldPos.w());

	Math::vec4 rayDirection = rayWorldPos - invView.get_position();
	rayDirection = Math::vec4::normalize(rayDirection);

	return Math::line(rayWorldPos, rayDirection, length);
}

}