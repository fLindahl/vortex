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

		///Handle messages
		void HandleMessage(const Ptr<Messaging::Message>& msg);
		
		///Serialize property
		void Serialize(IO::Stream* stream);

		///Return the Physics::Rigidbody pointed to by this property
		const Ptr<Physics::RigidBody>& GetRigidBody() const { return this->rigidBody; }

		///Get for isKinematic
		bool IsKinematic() const;
		void SetKinematic(bool value);

		///Get for isDebris
		bool IsDebris() const;
		void SetIsDebris(bool value);

		///Update mass to new value
		void SetMass(const float& newMass);
		
		///Returns this rigidbody's mass
		float GetMass() const;
		
	private:
		///This entity won't be affected by physics forces but is still affected by collisions
		bool isKinematic;
		///This makes the rigidbody only collide with static objects. This is a lot less precise than "non-debris" physics and therefor less expensive.
		bool isDebris;

		///The physics-side rigidbody
		Ptr<Physics::RigidBody> rigidBody;
	};

}