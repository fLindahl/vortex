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
__DeclareClass(CubeMapEntity)

public:
	CubeMapEntity();
	~CubeMapEntity();

	void Activate();
	void Deactivate();

	void Update();

	void SetTransform(const Math::mat4 &t);
	
	void GenerateCubeMap();

	GLuint GetCubeMapSampler();

	bool IsLoaded();

	Ptr<Render::CubeMapNode> GetCubeMapNode();
	void SetGeometryProxy(const Ptr<Render::GeometryProxy>& proxy);
protected:
	Ptr<Render::CubeMapNode> cubemap;
};
}