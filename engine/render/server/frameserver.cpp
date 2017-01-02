#include "config.h"
#include "frameserver.h"
#include "renderdevice.h"
#include "render/resources/framepass.h"

namespace Render
{
	FrameServer::FrameServer()
	{
		
	}

	void FrameServer::SetupFramePasses()
	{
		//Setup the common frame passes. 
		//TODO: these should be moved to a loader and their own xml list like everything else.

		// depth pre-pass
		this->Depth = std::make_shared<FramePass>();
		this->Depth->name = "Depth";
		glGenFramebuffers(1, &this->Depth->frameBufferObject);
		glGenTextures(1, &this->Depth->buffer);

		glBindTexture(GL_TEXTURE_2D, this->Depth->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, this->Depth->frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->Depth->buffer, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->framePassByName.insert(std::make_pair(this->Depth->name, this->Depth));

		// Setup light culling compute shader program
		this->lightCullingProgram = glCreateProgram();
		const char filepath[] = "resources/shaders/compute/lightculling.comp";
		glAttachShader(this->lightCullingProgram, ShaderServer::Instance()->LoadComputeShader(filepath));
		glLinkProgram(this->lightCullingProgram);
		GLint shaderLogSize;
		glGetProgramiv(this->lightCullingProgram, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->lightCullingProgram, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
				
		// FlatGeometryLit pass
		this->FlatGeometryLit = std::make_shared<FramePass>();
		this->FlatGeometryLit->name = "FlatGeometryLit";
		this->FlatGeometryLit->frameBufferObject = 0;
		this->FlatGeometryLit->buffer = 0;

		this->framePassByName.insert(std::make_pair(this->FlatGeometryLit->name, this->FlatGeometryLit));

		glGenFramebuffers(1, &this->finalColorFrameBufferObject);
		glGenTextures(1, &this->finalColorBuffer);

		glBindTexture(GL_TEXTURE_2D, this->finalColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, this->finalColorFrameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->finalColorBuffer, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->Depth->buffer, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameServer::UpdateResolutions()
	{
		//TODO: Loop through all framepasses and update each one of their resolutions if needed.

		const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();
		glBindTexture(GL_TEXTURE_2D, this->Depth->buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, newRes.x, newRes.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, this->finalColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	std::shared_ptr<FramePass> FrameServer::GetFramePass(const std::string& name)
	{
		if (this->HasPassNamed(name))
			return this->framePassByName[name];
		else
		{
			printf("ERROR: No framepass named %s!\n", name);
			assert(false);
			return nullptr;
		}
	}

	bool FrameServer::HasPassNamed(const std::string &nName)
	{
		if (this->framePassByName.count(nName) > 0)
			return true;
		else
			return false;
	}

}