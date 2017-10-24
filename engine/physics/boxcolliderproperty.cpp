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

	Math::vector BoxCollider::GetSize() const
	{
		return this->collider.cast<Physics::BoxCollider>()->GetSize();
	}

	void BoxCollider::SetSize(const Math::vector& halfextents)
	{
		this->collider.cast<Physics::BoxCollider>()->SetSize(halfextents);
	}

	void BoxCollider::Deactivate()
	{

	}

	void BoxCollider::Serialize(IO::Stream * stream)
	{

	}
}