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
		
        virtual void Setup();

		virtual void Execute();

	protected:
		friend class RenderDevice;
		friend class FrameServer;
		std::string name;
	};

}
