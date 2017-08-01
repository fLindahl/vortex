#pragma once
#include "core/refcounted.h"
#include <memory>
#include "foundation/math/bbox.h"
#include "fysik/surfacecollider.h"
#include "fysik/rigidbody.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include "rendermessages.h"

namespace Render
{
class ModelInstance;
class ShaderObject;

class GraphicsProperty : public Game::BaseProperty
{
__DeclareClass(GraphicsProperty)
public:
	GraphicsProperty();
	~GraphicsProperty();

    void Update();

	void Activate();
	void Deactivate();

	void HandleMessage(const Ptr<Messaging::Message>& msg);

	Ptr<Render::ModelInstance> getModelInstance() const;
	void setModelInstance(const Ptr<Render::ModelInstance>& inModelInstance);
	
	const Math::mat4& getModelMatrix() const;
	const Math::mat4& getInvModelMatrix() const;
	void setModelMatrix(const Math::mat4 &mat);

	bool GetCastShadows() const;
	void SetCastShadows(const bool& value);

	void Serialize(IO::Stream* stream);

private:
	Ptr<Render::ModelInstance> modelInstance;
	Math::mat4 modelMat;
	Math::mat4 invMat;

	bool castShadows;
};

}