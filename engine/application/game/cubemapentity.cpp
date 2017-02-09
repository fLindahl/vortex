#include "config.h"
#include "cubemapentity.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/debugrender/debugrenderer.h"

namespace Game
{

CubeMapEntity::CubeMapEntity()
{
	//Call baseclass first!
	ModelEntity::ModelEntity();
	this->cubemap = std::make_shared<Render::CubeMapNode>();
}
	
CubeMapEntity::~CubeMapEntity()
{
	delete this->gProperty;
}

void CubeMapEntity::Activate()
{
	ModelEntity::Activate();
	this->cubemap->Activate();
}

void CubeMapEntity::Deactivate()
{
	ModelEntity::Deactivate();
	this->cubemap->Deactivate();
}

void CubeMapEntity::SetTransform(const Math::mat4 &t)
{
	ModelEntity::SetTransform(t);
	this->cubemap->SetPosition(t.get_position());
}

void CubeMapEntity::GenerateCubeMap()
{
	this->cubemap->GenerateCubeMap();
}

GLuint CubeMapEntity::GetCubeMapSampler()
{
	return this->cubemap->GetCubeMap();
}

bool CubeMapEntity::IsLoaded()
{
	return this->cubemap->IsLoaded();
}

void CubeMapEntity::Update()
{
	Debug::DebugRenderer::Instance()->DrawCircle(this->GetTransform().get_position(), Math::quaternion::identity(), cubemap->InnerScale().x(), Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);
	Debug::DebugRenderer::Instance()->DrawCircle(this->GetTransform().get_position(), Math::quaternion::identity(), cubemap->OuterScale().x(), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);

	Debug::DebugRenderer::Instance()->DrawCircle(this->GetTransform().get_position(), Math::quaternion::rotationyawpitchroll(0.0f, 1.57075f, 0.0f), cubemap->InnerScale().x(), Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);
	Debug::DebugRenderer::Instance()->DrawCircle(this->GetTransform().get_position(), Math::quaternion::rotationyawpitchroll(0.0f, 1.57075f, 0.0f), cubemap->OuterScale().x(), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);

	Debug::DebugRenderer::Instance()->DrawCircle(this->GetTransform().get_position(), Math::quaternion::rotationyawpitchroll(1.57075f, 0.0f, 1.57075f), cubemap->InnerScale().x(), Math::vec4(1.0f, 0.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);
	Debug::DebugRenderer::Instance()->DrawCircle(this->GetTransform().get_position(), Math::quaternion::rotationyawpitchroll(1.57075f, 0.0f, 1.57075f), cubemap->OuterScale().x(), Math::vec4(0.0f, 1.0f, 0.0f, 1.0f), Debug::RenderMode::WireFrame, 2.0f);

	Entity::Update();
}

}