#pragma once
#include "rigidbody.h"
#include "foundation/util/array.h"
#include <map>
namespace Physics
{

struct Interval
{
    Game::PhysicsEntity* entity;
};

struct AxisPoint
{
    float val;
    int entityID;
};

class PhysicsDevice
{
private:
    PhysicsDevice();

public:
    static PhysicsDevice* Instance()
    {
        static PhysicsDevice instance;
        return &instance;
    }

    // C++ 11
    // Delete the methods we don't want.
    PhysicsDevice(PhysicsDevice const&) = delete;
    void operator=(PhysicsDevice const&) = delete;

    void AddRigidBody(std::shared_ptr<RigidBody> rBody);

    void Solve();

    int GetCurrentAABBSortAxis() { return this->sortAxis; }

    bool hasCollision;

private:
    double time;
    double frameTime;

    Math::point Support(const Math::point& dir, Game::PhysicsEntity* entity, const Math::mat4& invMat);
    bool GJK(Game::PhysicsEntity* E1, Game::PhysicsEntity* E2);

    void BroadPhase();
    void NarrowPhase();

    Util::Array<std::shared_ptr<RigidBody>> rigidBodies;

    // Potentially Colliding Entities
    Util::Array<std::pair<Game::PhysicsEntity*, Game::PhysicsEntity*>> PCEntities;


    bool DoSimplex(Util::Array<Math::point> &points, Math::point &D);

    //Used for broad phase sorting
    int sortAxis;
};
}