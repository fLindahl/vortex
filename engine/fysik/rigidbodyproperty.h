#pragma once
#include "core/refcounted.h"
#include "foundation/math/bbox.h"
#include "fysik/surfacecollider.h"
#include "fysik/rigidbody.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include "colliderproperty.h"

namespace Property
{
	class Rigidbody : public Property::Collider
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

		//const Ptr<Physics::SurfaceCollider>& GetCollider() const { return this->collider; }
	private:
		///This entity won't be affected by physics forces but is still affected by collisions
		bool isKinematic;
		///This makes the rigidbody only collide with static objects. This is a lot less precise than "non-debris" physics and therefor less expensive.
		bool isDebris;

		Ptr<Physics::RigidBody> rigidBody;
		//Ptr<Physics::SurfaceCollider> collider;
	};

}