#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "foundation/util/array.h"

namespace Render
{
	class Material;

	class FramePass
	{
	public:
		FramePass();
		virtual ~FramePass();

		/// get OGL Framebuffer object
		GLuint& GetFrameBufferObject() { return this->frameBufferObject; }

		/// Returns this framebuffer objects render texture
		GLuint& GetBuffer() { return this->buffer; }

		void AddMaterial(Material* mat) { this->materials.Append(mat); }

        virtual void Setup();

		void Bind();

		virtual void Execute();

	protected:
		friend class RenderDevice;
		friend class FrameServer;
		std::string name;

		/// Frame Buffer Object
		GLuint frameBufferObject;

		/// Texture
		GLuint buffer;

		Util::Array<Material*> materials;
	};

}