#pragma once

#include "entity.h"
#include "fysik/surfacecollider.h"

namespace Game
{
class StaticEntity : public Entity
{
public:
    StaticEntity();
    ~StaticEntity();

    void setCollider(std::shared_ptr<Physics::SurfaceCollider> coll) { this->collider = coll;}
    std::shared_ptr<Physics::SurfaceCollider> getCollider() { return this->collider;}

private:
    std::shared_ptr<Physics::SurfaceCollider> collider;

};

}