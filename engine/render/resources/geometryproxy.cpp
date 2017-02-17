#include "config.h"
#include "geometryproxy.h"
#include "application/basegamefeature/managers/envmanager.h"

namespace Render
{

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



}