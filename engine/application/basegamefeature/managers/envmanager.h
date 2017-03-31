#pragma once
#include "core/singleton.h"
#include "foundation/math/line.h"
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
#include "render/resources/geometryproxy.h"


namespace BaseGameFeature
{

class EnvManager
{
	__DeclareSingleton(EnvManager)

public:
	Math::line ComputeMouseWorldRay(const double& cursorX, const double& cursorY, const float& length, const float& viewPortSizeX, const float& viewPortSizeY);

	//Creates a new geometry proxy and adds it to a list
	const Ptr<Render::GeometryProxy>& NewGeometryProxy(Math::mat4 transform);

	//Renders all geometry proxies OBBs and icons
	//Can be called whenever, as they're rendered with debugrenderer, but has to be called each frame
	void RenderGeometryProxies();

private:
	friend class SceneManager;
	Util::Array<Ptr<Render::GeometryProxy>> geometryProxies;

};

}