#pragma once
#include "foundation/util/array.h"
#include "application/game/entity.h"

namespace Physics
{
class PhysicsServer
{
public:
    PhysicsServer();
    ~PhysicsServer();

    //Shoot a ray and return first object that it intersects.
    bool Raycast(shared_ptr<Game::Entity> out, const Math::Vector4& position, const Math::Vector4& direction /*, const ExcludeSet& exclude*/);

    //Shoot a ray and return all objects that it intersects.
    //bool Raycast(Util::Array<shared_ptr<Game::Entity>> out, const Math::Vector4& position, const Math::Vector4& direction /*, const ExcludeSet& exclude*/);


private:
    friend class PhysicsDevice;

    Util::Array<Game::Entity> entities;

};

}

