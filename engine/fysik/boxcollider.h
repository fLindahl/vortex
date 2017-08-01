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
__DeclareClass(BoxCollider);
public:
	BoxCollider();
	~BoxCollider();


private:
	Math::vector size;
};

}