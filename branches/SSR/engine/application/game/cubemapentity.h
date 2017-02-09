#pragma once
#include "modelentity.h"
#include "render/resources/cubemapnode.h"

namespace Render
{
class GraphicsProperty;
class ModelInstance;
}

namespace Game
{
class CubeMapEntity : public ModelEntity
{
public:
	CubeMapEntity();
	~CubeMapEntity();

	void Activate();
	void Deactivate();

	void SetTransform(const Math::mat4 &t);

	void GenerateCubeMap();

	GLuint GetCubeMapSampler();

	bool IsLoaded();

protected:
	std::shared_ptr<Render::CubeMapNode> cubemap;
};
}