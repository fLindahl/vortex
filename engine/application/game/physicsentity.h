#pragma once
#include "entity.h"


namespace Render
{
class GraphicsProperty;
}

namespace Physics
{
class SurfaceCollider;
}

namespace Game
{
class PhysicsEntity : public Entity
{
public:
    PhysicsEntity() {}
    ~PhysicsEntity() {}

    void SetCollider(std::shared_ptr<Physics::SurfaceCollider> coll) { this->collider = coll; }
    std::shared_ptr<Physics::SurfaceCollider> GetCollider() { return this->collider; }

    void SetGraphicsProperty(Render::GraphicsProperty* gp);

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

protected:
    Render::GraphicsProperty* gProperty;
    std::shared_ptr<Physics::SurfaceCollider> collider;

};
}