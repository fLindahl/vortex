#pragma once
/**
 * Physics::PhysicsServer
 * A singleton for various functionality that we want for our physics engine, such as raytracing, collision queries etc.
 */
#include "core/refcounted.h"
#include "foundation/util/array.h"
#include "application/game/entity.h"
#include "foundation/math/plane.h"
#include "foundation/math/line.h"
#include "colliderproperty.h"
#include "rigidbodyproperty.h"
#include <map>

namespace Game
{
//class PhysicsEntity;
}

namespace Physics
{

class BaseCollider;

struct DynamicsData
{
	float massInv = 0;
	Math::point linearVelocity = Math::point::zerovector();
	Math::point angularVelocity = Math::point::zerovector();
	Math::mat4 invInertiaTensorWorld = Math::mat4::zeromatrix();
	Math::point position = Math::point::zerovector();
};

struct PhysicsHit
{
    Game::Entity* object;
    Math::point point;
    Math::point surfaceNormal;
};

enum PhysicsType
{
	Static,
	Dynamic
};

///Used to store all physicsentites within the same array
struct PhysicsEntity
{
	uint entityID;
	PhysicsType type;

	///This contains either rigidbody property or just a collider property depending on if this is a dynamic or static entity
	Ptr<Game::BaseProperty> property;
};

class PhysicsServer
{
private:
    PhysicsServer();

public:
    static PhysicsServer* Instance()
    {
        static PhysicsServer instance;
        return &instance;
    }

    // C++ 11
    // Delete the methods we don't want.
    PhysicsServer(PhysicsServer const&) = delete;
    void operator=(PhysicsServer const&) = delete;

    ///Shoot a ray and return first object that it intersects.
    bool Raycast(PhysicsHit& out, const Math::line& ray/*, const ExcludeSet& exclude*/);
	bool Raycast(PhysicsHit& out, const Math::vec4& position, const Math::vec4& direction, const float& length/*, const ExcludeSet& exclude*/);

    void AddDynamicEntity(Ptr<Property::Rigidbody> p);
	void RemoveDynamicEntity(Ptr<Property::Rigidbody> p);

	void AddStaticEntity(Ptr<Property::Collider> p);
	void RemoveStaticEntity(Ptr<Property::Collider> p);

    ///Shoot a ray and return all objects that it intersects.
    //bool Raycast(Util::Array<PhysicsHit>& out, const Math::vec4& position, const Math::vec4& direction /*, const ExcludeSet& exclude*/);


    static Math::mat4 CalculateInertiaTensor(Ptr<BaseCollider> collider, const float& mass);

	///Loads a collider based on mesh name and collider type.
	///Note: If a collider has already been cooked from this mesh, it will never cook it again, eventhough you ask for a different shaped collider.
	///TODO: we should split it into multiple lists with different shapes so that we can load them separately.
	Ptr<Physics::BaseCollider> LoadCollider(const std::string& meshName, const Physics::ColliderShape& shape);

private:
    friend class PhysicsDevice;

    ///Check if point is within 4 given positions. Make sure a,b,c,d is in clockwise order.
    bool isPointWithinBounds(const Math::point& p, const Math::point& a, const Math::point& b, const Math::point& c, const Math::point& d, const Math::vec4& surfaceNormal);
    ///Check if point is within 3 given positions. Make sure a,b,c is in clockwise order.
    bool isPointWithinBounds(const Math::point& p, const Math::point& a, const Math::point& b, const Math::point& c, const Math::vec4& surfaceNormal);

    Util::Array<PhysicsEntity> physicsEntities;

	std::map<std::string, Ptr<Physics::BaseCollider>> collidersByMeshName;
};

}

