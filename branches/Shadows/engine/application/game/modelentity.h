#pragma once
#include "entity.h"

namespace Render
{
class GraphicsProperty;
class ModelInstance;
}

namespace Game
{

class ModelEntity : public Entity
{
public:
	ModelEntity();
	~ModelEntity();

	void SetModel(std::shared_ptr<Render::ModelInstance> mdl);

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();

	void SetTransform(const Math::mat4& t);

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

protected:
	Render::GraphicsProperty* gProperty;
};
}