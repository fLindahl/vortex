#include "config.h"
#include "colliderproperty.h"
#include "physics/physicsdevice.h"
#include "physics/physicsserver.h"
#include "render/properties/rendermessages.h"

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
		if (!this->active)
		{
			Physics::PhysicsServer::Instance()->AddStaticEntity(this);
			BaseProperty::Activate();
		}
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
		//Handle set transform message
		if (msg->GetType() == Msg::SetMesh::Type)
		{
			Ptr<Render::MeshResource> mesh = msg.cast<Msg::SetMesh>()->Get();
			this->collider = Physics::PhysicsServer::Instance()->LoadCollider(mesh->GetName(), Physics::ColliderShape::SURFACE).downcast<Physics::SurfaceCollider>();
		}
	}

	void Collider::Serialize(IO::Stream* stream)
	{

	}
}
