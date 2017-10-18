#pragma once
#include "core/refcounted.h"
#include "basecollider.h"
#include "application/game/baseproperty.h"
#include "physics/surfacecollider.h"

#include "btBulletDynamicsCommon.h"

namespace Render
{
	class GraphicsProperty;
}

namespace Physics
{

class RigidBody : public Core::RefCounted
{
__DeclareClass(RigidBody)
public:
    RigidBody();
    ~RigidBody();
	
	bool Initialize();
	bool Uninitialize();
		
	/**	Change the mass of the rigidbody.
		If the body is activated, this will deactivate it and re-initialize + re-activate it with the new mass.
		@note	This might perform badly since we seem to need to re-initialize the entire rigidbody. */
	void SetMass(float m);

	///Returns this rigidbody's mass.
	float GetMass() const;

	///Set this rigidbody's collider. 
	void SetCollider(const Ptr<BaseCollider>& collider);

	///Returns this rigidbody's collider. 
	Ptr<BaseCollider> GetCollider() const;

	void ApplyForce(const Math::vec4& dir, const float& magnitude);
	void ApplyForceAtPoint(const Math::vec4& dir, const float& magnitude, const Math::point& worldPos);
	
	bool IsInitialized() const { return this->initialized; }

private:
	friend class PhysicsDevice;

	bool initialized;
	bool registered;

	float mass;

	btRigidBody* body;
	btDefaultMotionState* motionState;

	Ptr<BaseCollider> collider;

	void ReloadBtRigidBody();
};

}