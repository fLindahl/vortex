#pragma once
#include "entity.h"


namespace Render
{
class GraphicsProperty;
class ModelInstance;
}

namespace Physics
{
class SurfaceCollider;

enum PhysicsType
{
	Static,
	Rigidbody
};

}

namespace Game
{

class PhysicsEntity : public Entity
{
public:
	PhysicsEntity();
	~PhysicsEntity();

    //void SetCollider(std::shared_ptr<Physics::SurfaceCollider> coll) { this->collider = coll; }
    std::shared_ptr<Physics::SurfaceCollider> GetCollider() { return this->collider; }

	void SetModel(std::shared_ptr<Render::ModelInstance> mdl);

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();

    Render::GraphicsProperty* GetGraphicsProperty() { return this->gProperty; }

	Physics::PhysicsType GetPhysicsType() const { return this->physicsType; }

	void SetTransform(const Math::mat4& t);

	//void SetPhysicsType(const Physics::PhysicsType& t) { this->physicsType = t; }

protected:
	Physics::PhysicsType physicsType;
    Render::GraphicsProperty* gProperty;
    std::shared_ptr<Physics::SurfaceCollider> collider;

};
}