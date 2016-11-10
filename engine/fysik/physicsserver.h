#pragma once
/**
 * Physics::PhysicsServer
 * A singleton for various functionality that we want for our physics engine, such as raytracing, collision queries etc.
 */

#include "foundation/util/array.h"
#include "application/game/entity.h"
#include "foundation/math/plane.h"

namespace Physics
{
class PhysicsServer
{
public:
    PhysicsServer();
    ~PhysicsServer();

    //Shoot a ray and return first object that it intersects.
    bool Raycast(std::shared_ptr<Game::Entity> out, const Math::vec4& position, const Math::vec4& direction, const float& length /*, const ExcludeSet& exclude*/);

    // TEMPORARY: Raycast and check against a list of specific objects.
    static bool Raycast(Math::vec4& out, const Math::vec4& position, const Math::vec4& direction, const float& length, const Math::plane& plane);


    //Shoot a ray and return all objects that it intersects.
    //bool Raycast(Util::Array<shared_ptr<Game::Entity>> out, const Math::vec4& position, const Math::vec4& direction /*, const ExcludeSet& exclude*/);


private:
    friend class PhysicsDevice;

    Util::Array<Game::Entity> entities;

};

}

