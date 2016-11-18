#include "config.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "physicsdevice.h"

namespace Physics
{
    PhysicsDevice::PhysicsDevice()
    {
        time = glfwGetTime();
    }

    void PhysicsDevice::Solve()
    {
        //-----TIME-----
        double currentTime = glfwGetTime();
        this->frameTime = currentTime - time;
        //--------------

        for(auto rigidbody : this->rigidBodies)
        {
            rigidbody->update(this->frameTime);
        }

        //EvaluateCollisions();

        //-----TIME-----
        this->time = currentTime;
        //--------------
    }
void PhysicsDevice::AddRigidBody(std::shared_ptr<RigidBody> rBody)
{
    this->rigidBodies.Append(rBody);

    float mass = 5.0f;
    float d = 0.083333333f * mass;
    Math::vec4 boxExtents = rBody->collider->getbbox().maxPoint;
    float sqX = (boxExtents.x() * boxExtents.x());
    float sqY = (boxExtents.y() * boxExtents.y());
    float sqZ = (boxExtents.z() * boxExtents.z());

    Math::mat4 inertiaTensor = Math::mat4(
        d * (sqY + sqZ), 0, 0, 0,
        0, d * (sqX + sqZ), 0, 0,
        0, 0, d * (sqX + sqY), 0,
        0, 0, 0, 1
    );

	rBody->initialize(5.0f, inertiaTensor);
}

}