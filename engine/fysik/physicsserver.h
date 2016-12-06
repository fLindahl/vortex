#pragma once
/**
 * Physics::PhysicsServer
 * A singleton for various functionality that we want for our physics engine, such as raytracing, collision queries etc.
 */

#include "application/game/physicsentity.h"
#include "foundation/util/array.h"
#include "application/game/entity.h"
#include "foundation/math/plane.h"
#include "foundation/math/line.h"

namespace Game
{
class PhysicsEntity;
}

namespace Physics
{

class BaseCollider;

enum ColliderShape
{
    SPHERE,
    CAPSULE,
    BOX,
    SURFACE
};

struct DynamicsData
{
	float massInv = 0;
	Math::vec4 linearVelocity = Math::vec4::zerovector();
	Math::vec4 angularVelocity = Math::vec4::zerovector();
	Math::mat4 invInertiaTensorWorld = Math::mat4::zeromatrix();
	Math::point position = Math::point::zerovector();
};

struct PhysicsHit
{
    Game::Entity* object;
    Math::point point;
    Math::point surfaceNormal;
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
    bool Raycast(PhysicsHit& out, const Math::vec4& position, const Math::vec4& direction, const float& length /*, const ExcludeSet& exclude*/);

    void addPhysicsEntity(Game::PhysicsEntity* p);

    ///Shoot a ray and return all objects that it intersects.
    //bool Raycast(Util::Array<PhysicsHit>& out, const Math::vec4& position, const Math::vec4& direction /*, const ExcludeSet& exclude*/);


    static Math::mat4 CalculateInertiaTensor(std::shared_ptr<BaseCollider> collider, const float& mass);
private:
    friend class PhysicsDevice;

    ///Check if point is within 4 given positions. Make sure a,b,c,d is in clockwise order.
    bool isPointWithinBounds(const Math::point& p, const Math::point& a, const Math::point& b, const Math::point& c, const Math::point& d, const Math::vec4& surfaceNormal);
    ///Check if point is within 3 given positions. Make sure a,b,c is in clockwise order.
    bool isPointWithinBounds(const Math::point& p, const Math::point& a, const Math::point& b, const Math::point& c, const Math::vec4& surfaceNormal);

    Util::Array<Game::PhysicsEntity*> physicsEntities;
};

}

