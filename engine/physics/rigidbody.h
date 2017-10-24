#pragma once
#include "core/refcounted.h"
#include "basecollider.h"
#include "application/game/baseproperty.h"
#include "physics/basecollider.h"

class btDefaultMotionState;
class btRigidBody;


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
	
	bool Initialize(const Math::mat4& startTransform);
	bool Uninitialize();
		
	/**	Change the mass of the rigidbody.
		If the body is activated, this will deactivate it and re-initialize + re-activate it with the new mass.
		@note	This might perform badly since we seem to need to re-initialize the entire rigidbody. */
	void SetMass(float m);

	bool IsStatic() const;
	void SetStatic(bool val);

	///Returns this rigidbody's mass.
	float GetMass() const;

	///Set this rigidbody's collider. 
	void SetCollider(const Ptr<BaseCollider>& collider);

	///Returns the bullet-rigidbody's transform in worldspace
	Math::mat4 GetStateTransform() const;
	
	///Sets the bullet-rigidbody's worldspace transform
	void SetStateTransform(const Math::mat4& m);

	///Returns this rigidbody's collider. 
	Ptr<BaseCollider> GetCollider() const;

	void ApplyForce(const Math::vec4& dir, const float& magnitude);
	void ApplyForceAtPoint(const Math::vec4& dir, const float& magnitude, const Math::point& worldPos);
	
	bool IsInitialized() const { return this->initialized; }

private:
	friend class PhysicsDevice;

	bool initialized;
	bool isStatic;
	bool registered;

	float mass;

	btRigidBody* body;
	btDefaultMotionState* motionState;

	Ptr<BaseCollider> collider;

	void ReloadBtRigidBody();
};

}