#pragma once

/*---------------
 * Rendering Device
 *
 * Handles all rendering
 */

#include <unordered_map>
#include "GLFW/glfw3.h"
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"


namespace Render
{

    class ShaderObject;

    class RenderDevice
    {
    private:
        RenderDevice()
        {
            glGenBuffers(1, this->ubo);
        }

        struct UniformBufferBlock
        {
            Math::Matrix4 View;
            Math::Matrix4 Projection;

        } uniformBufferBlock;

        //Uniform Buffer Object
        GLuint ubo[1];

    public:
        static RenderDevice* Instance()
        {
            static RenderDevice instance;
            return &instance;
        }

        // C++ 11
        // Delete the methods we don't want.
        RenderDevice(RenderDevice const&) = delete;
        void operator=(RenderDevice const&) = delete;


        void addShaderObject(ShaderObject* obj) { this->shaderObjects.Append(obj); }
        //This renders all graphicsproperties.
        void Render();

    private:
        //contains all the shader objects that we've loaded
        Util::Array<ShaderObject*> shaderObjects;
    };


}