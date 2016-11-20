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
}

}