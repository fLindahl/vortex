#pragma once

/*---------------
 * Rendering Device
 *
 * Handles all rendering
 */
#include "core/refcounted.h"
#include "core/singleton.h"
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
		__DeclareSingleton(RenderDevice)

    private:        
        struct UniformBufferBlock
        {
            Math::mat4 View;
            Math::mat4 Projection;
            Math::mat4 ViewProjection;
			Math::mat4 InvView;
			Math::mat4 InvProjection;
			Math::mat4 InvViewProjection;
			Math::mat4 viewToTextureSpace;
			Math::vec4 CameraPos;
			Resolution ScreenSize;
			GLfloat TimeAndRandom[2];
			GLuint LightTileWorkGroups[2];
			
        } uniformBufferBlock;
		
        //Uniform Buffer Object
        GLuint ubo[1];

	public:
		///Returns render resolution
		const Resolution& GetRenderResolution() const { return this->renderResolution; }
		///Returns window resolution
		const Resolution& GetWindowResolution() const { return this->windowResolution; }

		///Update Render Resolution. This is separate from window resolution.
		void SetRenderResolution(const Resolution& res);
		void SetRenderResolution(const int& x, const int& y);

		///Update window resolution. NOTE: This is usually done automatically
		void SetWindowResolution(const int& x, const int& y);

		///Set uniform buffer from camera
		void SetUniformBuffer(const Graphics::Camera* camera);

		///Render the frame
		void Render(bool drawToScreen = true);

		///Render to a specified texture. Remember that you might need to set render resolution before doing this!
		void RenderToTexture(const GLuint& outFrameBuffer, const Graphics::Camera& camera);

		///Start up the render device.
		void Initialize();

		void SetFinalColorBuffer(GLuint cbuffer) { this->finalColorBuffer = cbuffer; }
		GLuint GetFinalColorBuffer() { return this->finalColorBuffer; }

		void SetPickingEnabled(const bool& val) { this->pickingEnabled = val; }
		bool GetPickingEnabled() { return this->pickingEnabled; }

   private:
		friend class LightServer;

		bool pickingEnabled;

		Resolution renderResolution;
		Resolution windowResolution;

		GLuint currentProgram;

		GLuint finalColorBuffer = 0;
		
    };
}