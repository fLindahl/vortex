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
#include "render/resources/material.h"


namespace Render
{

    class ShaderObject;

    class RenderDevice
    {
    private:
        RenderDevice()
        {
            glGenBuffers(1, this->ubo);
			currentProgram = 0;
        }

        struct UniformBufferBlock
        {
            Math::mat4 View;
            Math::mat4 Projection;
            Math::mat4 ViewProjection;

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


		void AddMaterial(Material* obj) { this->materials.InsertSorted(obj); }
        //This renders all graphicsproperties.
        void Render();

    private:
		GLuint currentProgram;

        //contains all the shader objects that we've loaded
        Util::Array<Material*> materials;
    };


}