#pragma once
#include "core/refcounted.h"
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
__DeclareClass(GraphicsProperty);
public:
	GraphicsProperty();
	~GraphicsProperty();

    void Update();

	void Activate();
	void Deactivate();

	Ptr<Render::ModelInstance> getModelInstance() const;
	void setModelInstance(const Ptr<Render::ModelInstance>& inModelInstance);
	
	const Math::mat4& getModelMatrix() const;
	const Math::mat4& getInvModelMatrix() const;
	void setModelMatrix(const Math::mat4 &mat);

	Math::bbox& getbbox() { return this->bbox; }

private:
	Math::bbox bbox;

	Ptr<Render::ModelInstance> modelInstance;
	Math::mat4 modelMat;
	Math::mat4 invMat;
};

}