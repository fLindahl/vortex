#include "config.h"
#include "rigidbodyproperty.h"
#include "render/resources/meshresource.h"
#include "render/resources/modelinstance.h"
#include "physics/physicsdevice.h"
#include "physics/physicsserver.h"
#include "render/properties/rendermessages.h"
#include "colliderproperty.h"

namespace Property
{
	__ImplementClass(Property::Rigidbody, 'PrRB', Game::BaseProperty)

	Rigidbody::Rigidbody() :
		isKinematic(false),
		isDebris(false)
	{
		this->rigidBody = Physics::RigidBody::Create();
	}

	Rigidbody::~Rigidbody()
	{
		
	}

	void Rigidbody::Update()
	{
		if (!this->active)
			return;

		//TODO: This should be a message, sent by the rigidbody
		this->owner->SetTransform(this->rigidBody->GetStateTransform());
		
		this->Game::BaseProperty::Update();
	}

	void Rigidbody::Activate()
	{
		if (!this->active)
		{
			if (!this->rigidBody->IsInitialized())
			{
				// Make sure this entity has a colliderproperty of some kind since we need it to initialize our rigidbody.
				const Ptr<Collider>& collider = this->owner->FindProperty<Property::Collider>();
				
				if (collider.isvalid())
				{
					this->rigidBody->SetCollider(collider->GetCollider());
					this->rigidBody->Initialize();
				}
				else
				{
					_warning("Cannot initialize Rigidbody property since it has no collider!");
				}
			}

			if (this->rigidBody->IsInitialized())
			{
				Physics::PhysicsDevice::Instance()->AddRigidBody(this->rigidBody);
				Physics::PhysicsServer::Instance()->AddDynamicEntity(this);
			}
			else
			{
				_warning("Cannot register rigidbody to PhysicsDevice since it has not been initialized properly!");
			}

			BaseProperty::Activate();
		}
	}

	void Rigidbody::Deactivate()
	{
		if (this->active)
		{
			if (this->rigidBody->IsInitialized())
			{
				Physics::PhysicsDevice::Instance()->RemoveRigidBody(this->rigidBody);
				Physics::PhysicsServer::Instance()->RemoveDynamicEntity(this);
			}
			BaseProperty::Deactivate();
		}
	}

	void Rigidbody::HandleMessage(const Ptr<Messaging::Message>& msg)
	{
		//Handle set transform message
		if (msg->GetType() == Msg::SetTransform::Type)
		{
			//this->rigidBody->SetStateTransform(msg.cast<Msg::SetTransform>()->Get());
		}
		else if (msg->GetType() == Msg::SetMesh::Type)
		{
			
		}
	}

	void Rigidbody::Serialize(IO::Stream* stream)
	{

	}

	bool Rigidbody::IsKinematic() const
	{
		return this->isKinematic;
	}

	void Rigidbody::SetKinematic(bool value)
	{
		this->isKinematic = value;
	}

	bool Rigidbody::IsDebris() const
	{
		return this->isDebris;
	}

	void Rigidbody::SetIsDebris(bool value)
	{
		this->isDebris = value;
	}

	void Rigidbody::SetMass(const float & newMass)
	{
		this->rigidBody->SetMass(newMass);
	}

	float Rigidbody::GetMass() const
	{
		return this->rigidBody->GetMass();
	}
}
