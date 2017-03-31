#include "config.h"
#include "geometryproxyentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/debugrender/debugrenderer.h"
#include "application/basegamefeature/managers/envmanager.h"
#include "render/resources/geometryproxy.h"

namespace Game
{

__ImplementClass(Game::GeometryProxyEntity, 'gepe', Game::ModelEntity)

GeometryProxyEntity::GeometryProxyEntity () : ModelEntity()
{
	//Call baseclass first
	this->proxy = BaseGameFeature::EnvManager::Instance()->NewGeometryProxy(this->transform);
}
	
GeometryProxyEntity::~GeometryProxyEntity()
{
	delete this->gProperty;
}

void GeometryProxyEntity::Activate()
{
	ModelEntity::Activate();
	this->proxy->Activate();
}

void GeometryProxyEntity::Deactivate()
{
	ModelEntity::Deactivate();
	this->proxy->Deactivate();
}

void GeometryProxyEntity::SetTransform(const Math::mat4 &t)
{
	ModelEntity::SetTransform(t);
	this->proxy->Transform().set_position(t.get_position());
}

Math::vec4 GeometryProxyEntity::GetScale()
{
	Math::vec4 scale;
	scale.x() = this->proxy->Transform().row0()[0]; 
	scale.y() = this->proxy->Transform().row1()[1];
	scale.z() = this->proxy->Transform().row2()[2];
	scale.w() = 1.0f;
	return scale;
}

void GeometryProxyEntity::SetScale(Math::vec4 scale)
{
	this->proxy->Transform().row0()[0] = scale.x();
	this->proxy->Transform().row1()[1] = scale.y();
	this->proxy->Transform().row2()[2] = scale.z();
}

Ptr<Render::GeometryProxy> GeometryProxyEntity::GetGeometryProxy()
{
	return this->proxy;
}

}