#include "config.h"
#include "colliderproperty.h"
#include "physics/physicsdevice.h"
#include "physics/physicsserver.h"
#include "render/properties/rendermessages.h"
#include "render/resources/meshresource.h"

namespace Property
{
	__ImplementClass(Property::Collider, 'Coll', Game::BaseProperty)

	Collider::Collider()
	{
	}

	Collider::~Collider()
	{
		
	}

	void Collider::Update()
	{
		if (!this->active)
			return;

		this->Game::BaseProperty::Update();
	}

	void Collider::Activate()
	{
		BaseProperty::Activate();

		//this->collider->SetBTUserPointer(this->owner);

		const Ptr<Property::Rigidbody>& rigidbody = this->owner->FindProperty<Property::Rigidbody>();

		//If no rigidbody is attached to the entity, we register it as a static collision object instead.
		if (!rigidbody.isvalid())
		{
			this->collider->InitializeStaticObject(this->owner->GetTransform());
		}
	}

	void Collider::Deactivate()
	{
		if (this->active)
		{
			//Uninitialize if static. This will do nothing if the object is not registered.
			this->collider->UninitializeStaticObject();
			BaseProperty::Deactivate();
		}
	}

	void Collider::HandleMessage(const Ptr<Messaging::Message>& msg)
	{
		
	}

	void Collider::Serialize(IO::Stream* stream)
	{

	}
}
