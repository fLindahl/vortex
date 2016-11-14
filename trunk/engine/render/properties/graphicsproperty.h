#pragma once
#include <memory>
#include <foundation/math/bbox.h>
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"

namespace Render
{
	class ModelInstance;
	class ShaderObject;

class GraphicsProperty
{
public:
	GraphicsProperty();
	~GraphicsProperty();

	std::shared_ptr<ModelInstance> getModelInstance() const;
	void setModelInstance(std::shared_ptr<ModelInstance> inModelInstance);
	
	Math::mat4 getModelMatrix() const;
	void setModelMatrix(const Math::mat4 &mat);

	Math::bbox& getbbox() {return this->bbox; }

private:
	Math::bbox bbox;

	std::shared_ptr<ModelInstance> modelInstance;
	Math::mat4 modelMat;
};

}