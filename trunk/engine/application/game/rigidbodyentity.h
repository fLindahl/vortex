#pragma once

#include "render/properties/graphicsproperty.h"
#include "fysik/physicsserver.h"
#include "entity.h"
#include "fysik/surfacecollider.h"
#include "fysik/rigidbody.h"

namespace Game
{
class RigidBodyEntity : public Entity
{
public:
    RigidBodyEntity();
    ~RigidBodyEntity();

    void Activate();
    void Deactivate();

    void SetCollider(std::shared_ptr<Physics::SurfaceCollider> coll) { this->collider = coll; }
    std::shared_ptr<Physics::SurfaceCollider> GetCollider() { return this->collider; }

    void Update();
    void FixedUpdate();

    void SetGraphicsProperty(std::shared_ptr<Property::GraphicsProperty> gp) {this->gProperty = gp};
    std::shared_ptr<Property::GraphicsProperty> GetGraphicsProperty() { return this->gProperty; }

    void SetTransform(const Math::mat4& t);

    std::shared_ptr<Physics::RigidBody> GetRigidBody() { return this->rigidBody; }

private:
    std::shared_ptr<Property::GraphicsProperty> gProperty;
    std::shared_ptr<Physics::RigidBody> rigidBody;
    std::shared_ptr<Physics::SurfaceCollider> collider;

};
}