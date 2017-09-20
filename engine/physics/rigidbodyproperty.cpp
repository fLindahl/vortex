#include "config.h"
#include "rigidbodyproperty.h"
#include "render/resources/meshresource.h"
#include "render/resources/modelinstance.h"
#include "fysik/physicsdevice.h"
#include "fysik/physicsserver.h"
#include "render/properties/rendermessages.h"

namespace Property
{
	__ImplementClass(Property::Rigidbody, 'PrRB', Property::Collider)

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
		this->owner->SetTransform(Math::mat4::multiply(Math::mat4::translation(this->rigidBody->getCenterOfMass() * -1.0f), this->rigidBody->getTransform()));

		this->Game::BaseProperty::Update();
	}

	void Rigidbody::Activate()
	{
		if (!this->active)
		{
			if (!this->rigidBody->IsInitialized())
			{
				if (this->collider.isvalid())
				{
					this->rigidBody->Initialize(1.0f, Physics::PhysicsServer::CalculateInertiaTensor(this->collider.upcast<Physics::BaseCollider>(), 1.0f), this->owner);
				}
				else
				{
					_warning("Cannot initialize Rigidbody property since no mesh has been set!");
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
			//this->setTransform(msg.cast<Msg::SetTransform>()->Get());
		}
		else if (msg->GetType() == Msg::SetMesh::Type)
		{
			if (this->active)
			{
				this->Deactivate();
				this->collider = Physics::PhysicsServer::Instance()->LoadCollider(msg.cast<Msg::SetMesh>()->Get()->GetName(), Physics::ColliderShape::SURFACE).downcast<Physics::SurfaceCollider>();
				this->Activate();
			}
			else
			{
				this->collider = Physics::PhysicsServer::Instance()->LoadCollider(msg.cast<Msg::SetMesh>()->Get()->GetName(), Physics::ColliderShape::SURFACE).downcast<Physics::SurfaceCollider>();
			}
			//this->setTransform(msg.cast<Msg::SetTransform>()->Get());
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
		//TODO: Fix this
	}

	float Rigidbody::GetMass() const
	{
		//TODO: Fix this
		return 0.0f;
	}
}
