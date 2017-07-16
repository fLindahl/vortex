#include "config.h"
#include "rigidbodyproperty.h"
#include "render/resources/meshresource.h"
#include "render/resources/modelinstance.h"
#include "fysik/physicsdevice.h"
#include "fysik/physicsserver.h"

namespace Property
{
	__ImplementClass(Property::Rigidbody, 'PrRB', Game::BaseProperty)

	Rigidbody::Rigidbody() : 
		isKinematic(false)
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
				//TODO: this
				//this->rigidBody->Initialize(1.0f, Physics::PhysicsServer::CalculateInertiaTensor(this->collider.downcast<Physics::BaseCollider>(), 1.0f), this->owner);
			}
			Physics::PhysicsDevice::Instance()->AddRigidBody(this->rigidBody);
			//TODO: this
			//Physics::PhysicsServer::Instance()->AddPhysicsEntity(this->owner);
			BaseProperty::Activate();
		}
	}

	void Rigidbody::Deactivate()
	{
		if (this->active)
		{
			Physics::PhysicsDevice::Instance()->RemoveRigidBody(this->rigidBody);
			//TODO: this
			//Physics::PhysicsServer::Instance()->removePhysicsEntity(this->owner);
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
	}

	void Rigidbody::Serialize(IO::Stream* stream)
	{

	}
}
