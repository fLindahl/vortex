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
            rigidbody->calculate(this->frameTime);
        }

        //EvaluateCollisions();



        //-----TIME-----
        this->time = currentTime;
        //--------------
    }

}