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
	__DeclareClass(PhysicsEntity)
public:
	PhysicsEntity();
	~PhysicsEntity();

    //void SetCollider(Ptr<Physics::SurfaceCollider> coll) { this->collider = coll; }
	Ptr<Physics::SurfaceCollider> GetCollider();

	void SetModel(Ptr<Render::ModelInstance> mdl);

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
    Ptr<Physics::SurfaceCollider> collider;

};
}