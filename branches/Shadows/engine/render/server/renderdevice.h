#pragma once

/*---------------
 * Rendering Device
 *
 * Handles all rendering
 */

#include <unordered_map>
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
#include "render/resources/material.h"

//#define _LIGHT_DEBUG

namespace Render
{
	struct Resolution
	{
		GLint x;
		GLint y;
	};

    class ShaderObject;

    class RenderDevice
    {
    private:
		RenderDevice();
        
        struct UniformBufferBlock
        {
            Math::mat4 View;
            Math::mat4 Projection;
            Math::mat4 ViewProjection;
			Math::mat4 InvView;
			Math::mat4 InvProjection;
			Math::mat4 InvViewProjection;
			Math::vec4 CameraPos;
			Resolution ScreenSize;
			GLfloat TimeAndRandom[2];
			GLuint LightTileWorkGroups[2];
			
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

		const Resolution& GetRenderResolution() const { return this->renderResolution; }
		const Resolution& GetWindowResolution() const { return this->windowResolution; }
		void SetRenderResolution(const Resolution& res);
		void SetRenderResolution(const int& x, const int& y);

		void SetWindowResolution(const int& x, const int& y);

		void AddMaterial(Material* obj) { this->materials.Append(obj); }
        //This renders all graphicsproperties.
        void Render(bool drawToScreen = true);

		void Initialize();

		void SetFinalColorBuffer(GLuint cbuffer) { this->finalColorBuffer = cbuffer; }
		GLuint GetFinalColorBuffer() { return this->finalColorBuffer; }

   private:
		friend class LightServer;

		Resolution renderResolution;
		Resolution windowResolution;

		GLuint currentProgram;

		GLuint finalColorBuffer = 0;
		
        //contains all the shader objects that we've loaded
        Util::Array<Material*> materials;
    };
}