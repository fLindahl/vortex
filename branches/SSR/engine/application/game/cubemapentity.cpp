#include "config.h"
#include "cubemapentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Game
{

CubeMapEntity::CubeMapEntity()
{
	//Call baseclass first!
	ModelEntity::ModelEntity();
}
	
CubeMapEntity::~CubeMapEntity()
{
	delete this->gProperty;
}

void CubeMapEntity::Activate()
{
	ModelEntity::Activate();


}

void CubeMapEntity::Deactivate()
{
	ModelEntity::Deactivate();
}

void CubeMapEntity::SetTransform(const Math::mat4 &t)
{
	ModelEntity::SetTransform(t);
	this->cubemap.SetPosition(t.get_position());
}

void CubeMapEntity::GenerateCubeMap()
{
	this->cubemap.GenerateCubeMap();
}

GLuint CubeMapEntity::GetCubeMapSampler()
{
	return this->cubemap.GetCubeMap();
}

bool CubeMapEntity::IsLoaded()
{
	return this->cubemap.IsLoaded();
}

}