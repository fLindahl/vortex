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
	}

	void Collider::Deactivate()
	{
		if (this->active)
		{
			Physics::PhysicsServer::Instance()->RemoveStaticEntity(this);
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
