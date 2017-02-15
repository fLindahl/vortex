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
class BoxEntity : public ModelEntity
{
public:
	BoxEntity();
	~BoxEntity();

	void SetModel(std::shared_ptr<Render::ModelInstance> mdl);

	void Activate();
	void Deactivate();

	void Update();

	void SetTransform(const Math::mat4 &t);
	
	void GenerateCubeMap();

	GLuint GetCubeMapSampler();

	bool IsLoaded();

	Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

protected:
	Render::GraphicsProperty* gProperty;
};
}