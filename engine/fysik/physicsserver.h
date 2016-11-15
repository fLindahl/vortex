#pragma once
/**
 * Physics::PhysicsServer
 * A singleton for various functionality that we want for our physics engine, such as raytracing, collision queries etc.
 */

#include "foundation/util/array.h"
#include "application/game/entity.h"
#include "foundation/math/plane.h"
#include "foundation/math/line.h"

namespace Physics
{

struct PhysicsHit
{
    Render::GraphicsProperty* object;
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

    void addGraphicsProperty(Render::GraphicsProperty* p) { this->properties.Append(p);}

    ///Shoot a ray and return all objects that it intersects.
    //bool Raycast(Util::Array<PhysicsHit>& out, const Math::vec4& position, const Math::vec4& direction /*, const ExcludeSet& exclude*/);


private:
    friend class PhysicsDevice;

    ///Check if point is within 4 given positions. Make sure a,b,c,d is in clockwise order.
    bool isPointWithinBounds(const Math::point& p, const Math::point& a, const Math::point& b, const Math::point& c, const Math::point& d, const Math::vec4& surfaceNormal);
    ///Check if point is within 3 given positions. Make sure a,b,c is in clockwise order.
    bool isPointWithinBounds(const Math::point& p, const Math::point& a, const Math::point& b, const Math::point& c, const Math::vec4& surfaceNormal);

    Util::Array<Render::GraphicsProperty*> properties;

};

}

