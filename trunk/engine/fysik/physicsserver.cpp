#include <foundation/math/line.h>
#include "config.h"
#include "physicsserver.h"

namespace Physics
{
PhysicsServer::PhysicsServer()
{

}

PhysicsServer::~PhysicsServer()
{

}

bool PhysicsServer::Raycast(shared_ptr<Game::Entity> out, const Math::Vector4 &position, const Math::Vector4 &direction, const float& length)
{
    //Create line from position, in length * direction
    Math::Line ray = Math::Line(position, direction * length);

    //for each object, check AABB for collision
    //for (int i = 0; i < this->entities.Size(); ++i)
    //{
            // Get AABB and check for hit
    //}
}

bool PhysicsServer::Raycast(Math::Vector4& out,
                            const Math::Vector4 &position,
                            const Math::Vector4 &direction,
                            const float &length,
                            const Math::Plane &plane)
{
    Math::Line ray = Math::Line(position, direction, length);

    Math::Vector4 hit;
    if(ray.Intersect(hit, plane))
    {
        //printf("Hit plane at %f, %f, %f, %f\n", hit.x(), hit.y(), hit.z(), hit.w());
        out = hit;
        return true;
    }
    else
    {
        return false;
    }
}

}