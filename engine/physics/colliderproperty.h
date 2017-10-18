#pragma once
#include "core/refcounted.h"
#include "foundation/math/bbox.h"
#include "physics/basecollider.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include "application/game/baseproperty.h"

namespace Property
{
	class Collider : public Game::BaseProperty
	{
		__DeclareClass(Collider);
	public:
		Collider();
		~Collider();

		void Update();

		void Activate();
		void Deactivate();

		void HandleMessage(const Ptr<Messaging::Message>& msg);

		void Serialize(IO::Stream* stream);

		const Ptr<Physics::BaseCollider>& GetCollider() const { return this->collider; }
	protected:

		Ptr<Physics::BaseCollider> collider;
	};

}