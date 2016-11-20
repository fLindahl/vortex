#include "config.h"
#include "physicsentity.h"
#include "render/properties/graphicsproperty.h"

namespace Game
{

void PhysicsEntity::SetGraphicsProperty(Render::GraphicsProperty* gp)
{
    this->gProperty = gp;
    this->gProperty->SetOwner(this);
}
}