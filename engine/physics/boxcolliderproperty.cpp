#include "config.h"
#include "boxcolliderproperty.h"
#include "boxcollider.h"

namespace Property
{
	__ImplementClass(Property::BoxCollider, 'prBC', Property::Collider)

	BoxCollider::BoxCollider()
	{
		this->collider = Physics::BoxCollider::Create();
	}

	BoxCollider::~BoxCollider()
	{

	}

	void BoxCollider::Update()
	{

	}

	void BoxCollider::Activate()
	{
		Collider::Activate();
	}

	void BoxCollider::Deactivate() 
	{
		Collider::Deactivate();
	}

	Math::vector BoxCollider::GetSize() const
	{
		return this->collider.cast<Physics::BoxCollider>()->GetSize();
	}

	void BoxCollider::SetSize(const Math::vector& halfextents)
	{
		this->collider.cast<Physics::BoxCollider>()->SetSize(halfextents);
	}
	
	void BoxCollider::Serialize(IO::Stream * stream)
	{

	}
}