#pragma once
#include "modelentity.h"
#include "render/resources/geometryproxy.h"

namespace Render
{
class GraphicsProperty;
class ModelInstance;
}

namespace Game
{
class GeometryProxyEntity : public ModelEntity
{
public:
	GeometryProxyEntity();
	~GeometryProxyEntity();

	void Activate();
	void Deactivate();

	void SetTransform(const Math::mat4 &t);

	Math::vec4 GetScale();
	void SetScale(Math::vec4 scale);
	
	std::shared_ptr<Render::GeometryProxy> GetGeometryProxy() { return this->proxy; }

protected:
	std::shared_ptr<Render::GeometryProxy> proxy;
};
}