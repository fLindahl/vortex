#include "config.h"
#include "framepass.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Render
{
	FramePass::FramePass()
	{
	
	}

	FramePass::~FramePass()
	{
		
	}

	void FramePass::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	}
}