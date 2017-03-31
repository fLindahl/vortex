#include "config.h"
#include "geometryproxy.h"
#include "cubemapnode.h"

namespace Render
{
__ImplementClass(Render::GeometryProxy, 'GEOP', Core::RefCounted);

GeometryProxy::GeometryProxy() :
		isActive(false)
{
	transform = Math::mat4::identity();
}

GeometryProxy::GeometryProxy(Math::mat4 transform) :
	isActive(false)
{
	this->transform = transform;
}



GeometryProxy::~GeometryProxy()
{
}

void GeometryProxy::Activate()
{
	if (!this->isActive)
	{
		this->isActive = true;
	}
}

void GeometryProxy::Deactivate()
{
	if (this->isActive)
	{
		this->isActive = false;
	}
}

const Util::Array<Ptr<CubeMapNode>>& GeometryProxy::GetConnectedCubemaps() const
{ 
	return this->connectedCubemaps; 
}

void GeometryProxy::ConnectCubemap(Ptr<CubeMapNode> cubemap) 
{ 
	this->connectedCubemaps.Append(cubemap);
}

}