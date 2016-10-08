#pragma once
#include <memory>
#include "foundation/math/matrix4.h"
#include "foundation/math/matrix3.h"
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

	shared_ptr<ModelInstance> getModelInstance() const;
	void setModelInstance(shared_ptr<ModelInstance> inModelInstance);
	
	Math::Matrix4 getModelMatrix() const;
	void setModelMatrix(const Math::Matrix4 &mat);

private:
	shared_ptr<ModelInstance> modelInstance;
	Math::Matrix4 modelMat;
};

}