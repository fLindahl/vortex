#pragma once
#include "core/refcounted.h"
#include "foundation/math/bbox.h"
#include "physics/boxcollider.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include "application/game/baseproperty.h"
#include "colliderproperty.h"

namespace Property
{
	class BoxCollider : public Property::Collider
	{
		__DeclareClass(BoxCollider)
	public:
		BoxCollider();
		~BoxCollider();

		void Update();

		void Activate();
		void Deactivate();

		Math::vector GetSize() const;
		void SetSize(const Math::vector& halfextents);

		void Serialize(IO::Stream* stream);

	};

}