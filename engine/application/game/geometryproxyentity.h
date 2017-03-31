#pragma once
#include "modelentity.h"

namespace Render
{
class GraphicsProperty;
class ModelInstance;
class GeometryProxy;
}

namespace Game
{
class GeometryProxyEntity : public ModelEntity
{
__DeclareClass(GeometryProxyEntity);
public:
	GeometryProxyEntity();
	~GeometryProxyEntity();

	void Activate();
	void Deactivate();

	void SetTransform(const Math::mat4 &t);

	Math::vec4 GetScale();
	void SetScale(Math::vec4 scale);
	
	Ptr<Render::GeometryProxy> GetGeometryProxy();

protected:
	Ptr<Render::GeometryProxy> proxy;
};
}