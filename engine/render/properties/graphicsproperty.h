#pragma once
#include <memory>
#include "foundation/math/bbox.h"
#include "fysik/surfacecollider.h"
#include "fysik/rigidbody.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"

namespace Render
{
class ModelInstance;
class ShaderObject;

class GraphicsProperty : public Game::BaseProperty
{
public:
	GraphicsProperty();
	~GraphicsProperty();

    void Update();

	void Activate();
	void Deactivate();

	std::shared_ptr<Render::ModelInstance> getModelInstance() const;
	void setModelInstance(std::shared_ptr<Render::ModelInstance> inModelInstance);
	
	Math::mat4 getModelMatrix() const;
	void setModelMatrix(const Math::mat4 &mat);

	Math::bbox& getbbox() { return this->bbox; }

private:
	Math::bbox bbox;

	std::shared_ptr<Render::ModelInstance> modelInstance;
	Math::mat4 modelMat;
};

}