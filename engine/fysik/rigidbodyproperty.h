#pragma once
#include "core/refcounted.h"
#include <memory>
#include "foundation/math/bbox.h"
#include "fysik/surfacecollider.h"
#include "fysik/rigidbody.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include "application/game/baseproperty.h"

namespace Property
{
	class Rigidbody : public Game::BaseProperty
	{
		__DeclareClass(Rigidbody)
	public:
		Rigidbody();
		~Rigidbody();

		void Update();

		void Activate();
		void Deactivate();

		void HandleMessage(const Ptr<Messaging::Message>& msg);

		void Serialize(IO::Stream* stream);

		const Ptr<Physics::RigidBody>& GetRigidBody() const { return this->rigidBody; }
		
		const Ptr<Physics::BaseCollider>& GetCollider() const { return this->collider.upcast<Physics::BaseCollider>(); }

	private:
		bool isKinematic;

		Ptr<Physics::SurfaceCollider> collider;
		Ptr<Physics::RigidBody> rigidBody;
	};

}