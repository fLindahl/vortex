#pragma once
#include "core/refcounted.h"
#include "foundation/math/plane.h"
#include "basecollider.h"
#include "foundation/math/point.h"
#include "render/resources/meshresource.h"

namespace Physics
{
	

class BoxCollider : public BaseCollider
{	
__DeclareClass(BoxCollider)
public:
	BoxCollider();
	~BoxCollider();

	///Set the extents of the box.
	void SetSize(const Math::vector& localSize);
	Math::vector GetSize() const;

private:
	Math::vector size;
};

}