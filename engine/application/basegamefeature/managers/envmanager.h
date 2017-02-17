#pragma once
#include "foundation/math/line.h"
#include "foundation/math/matrix4.h"
#include "render/resources/geometryproxy.h"

namespace BaseGameFeature
{

class EnvManager
{
private:
	EnvManager()
	{

	}

public:
	static EnvManager* Instance()
	{
		static EnvManager instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	EnvManager(EnvManager const&) = delete;
	void operator=(EnvManager const&) = delete;

	Math::line ComputeMouseWorldRay(const double& cursorX, const double& cursorY, const float& length, const float& viewPortSizeX, const float& viewPortSizeY);

	//Creates a new geometry proxy and adds it to a list
	std::shared_ptr<Render::GeometryProxy> NewGeometryProxy(Math::mat4 transform);

	//Renders all geometry proxies OBBs and icons
	//Can be called whenever, as they're rendered with debugrenderer, but has to be called each frame
	void RenderGeometryProxies();

private:
	Util::Array<std::shared_ptr<Render::GeometryProxy>> geometryProxies;

};

}