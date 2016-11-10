#include "config.h"
#include "foundation/math/line.h"
#include "physicsserver.h"

namespace Physics
{
PhysicsServer::PhysicsServer()
{

}

PhysicsServer::~PhysicsServer()
{

}

bool PhysicsServer::Raycast(std::shared_ptr<Game::Entity> out, const Math::vec4 &position, const Math::vec4 &direction, const float& length)
{
    //Create line from position, in length * direction
    //Math::Line ray = Math::Line(position, direction * length);

    //for each object, check AABB for collision
    //for (int i = 0; i < this->entities.Size(); ++i)
    //{
            // Get AABB and check for hit
    //}

	return false;
}

bool PhysicsServer::Raycast(Math::vec4& out,
                            const Math::vec4& position,
                            const Math::vec4& direction,
                            const float& length,
                            const Math::plane& plane)
{
    Math::Line ray = Math::Line(position, direction, length);

    Math::vec4 hit;
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