#include "config.h"
#include "envmanager.h"
#include "render/server/renderdevice.h"
#include "render/debugrender/debugrenderer.h"

namespace BaseGameFeature
{

Math::line EnvManager::ComputeMouseWorldRay(const double& cursorX, const double& cursorY, const float& length, const float& viewPortSizeX, const float& viewPortSizeY)
{
	Render::Resolution res = Render::RenderDevice::Instance()->GetRenderResolution();
	Math::mat4 invView = Graphics::MainCamera::Instance()->getInvView();
	
	double cursorPosX;
	double cursorPosY;

	// Transform to world coordinates
	cursorPosX = (((cursorX / viewPortSizeX) - 0.5f) * 2.0f);
	cursorPosY = (((cursorY / viewPortSizeY) - 0.5f) * 2.0f);
	Math::vec4 cursorTransform = Math::vec4((float)cursorPosX, (float)cursorPosY, 1.0, 1.0f);

	cursorTransform = Math::mat4::transform(cursorTransform, Graphics::MainCamera::Instance()->getInvProjection());
	Math::point ray = (cursorTransform * 0.01f);
	Math::vec4 rayWorldPos = Math::mat4::transform(ray, invView);

	Math::vec4 rayDirection = rayWorldPos - invView.get_position();
	rayDirection = Math::vec4::normalize(rayDirection);

	return Math::line(rayWorldPos, rayDirection, length);
}

std::shared_ptr<Render::GeometryProxy> EnvManager::NewGeometryProxy(Math::mat4 transform)
{
	std::shared_ptr<Render::GeometryProxy> proxy = std::make_shared<Render::GeometryProxy>(transform);
	proxy->Activate();

	this->geometryProxies.Append(proxy);

	return proxy;
}

void EnvManager::RenderGeometryProxies()
{
	for (size_t i = 0; i < this->geometryProxies.Size(); ++i)
	{
		Debug::DebugRenderer::Instance()->DrawBox(this->geometryProxies[i]->Transform(), Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 1.0f);
	}
}

}