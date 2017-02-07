#include "config.h"
#include "cubemapnode.h"

namespace Render
{

CubeMapNode::CubeMapNode() :
		isLoaded(false),
		resolution({ 128, 128 }),
		mipLevels(6)		
{
	
}

CubeMapNode::~CubeMapNode()
{
}

void CubeMapNode::DeleteCubeMap()
{
	if (this->isLoaded)
	{
		glDeleteTextures(1, &this->cubeSampler);
	}
	this->isLoaded = false;
}

void CubeMapNode::SetPosition(Math::point pos)
{
	this->position = pos;
}

void CubeMapNode::GenerateCubeMap()
{
	if (!this->isLoaded)
	{
		glGenTextures(1, &this->cubeSampler);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeSampler);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		for (size_t i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, this->resolution.x, this->resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		
	}

	//Keep the old render resolution so that we can switch back to it later.
	Render::Resolution defaultRenderResolution = Render::RenderDevice::Instance()->GetRenderResolution();
	//Update render resolution
	Render::RenderDevice::Instance()->SetRenderResolution(this->resolution);

	Graphics::Camera camera = Graphics::Camera();
	//Set camera position
	camera.SetPosition(this->position);
	camera.SetFov(1.57075); //90 degree fov
	camera.UpdateProjectionMatrix();

	GLuint temporaryFrameBuffer;

	//Create Temporary framebuffer that we can render to
	glGenFramebuffers(1, &temporaryFrameBuffer);

	//Render to textures
	this->RenderTexture(temporaryFrameBuffer, CubeFace::RIGHT, camera);
	this->RenderTexture(temporaryFrameBuffer, CubeFace::LEFT, camera);
	this->RenderTexture(temporaryFrameBuffer, CubeFace::TOP, camera);
	this->RenderTexture(temporaryFrameBuffer, CubeFace::BOTTOM, camera);
	this->RenderTexture(temporaryFrameBuffer, CubeFace::BACK, camera);
	this->RenderTexture(temporaryFrameBuffer, CubeFace::FRONT, camera);

	//Delete FrameBuffer
	glDeleteFramebuffers(1, &temporaryFrameBuffer);

	RenderDevice::Instance()->SetRenderResolution(defaultRenderResolution);

	this->isLoaded = true;
}

void CubeMapNode::RenderTexture(const GLuint& framebuffer, CubeFace face, Graphics::Camera& camera)
{
	Math::point forward;
	Math::point up;

	switch (face)
	{
	case Render::CubeMapNode::TOP:
		forward = Math::point(0, 1, 0);
		up = Math::point(0, 0, 1);
		break;
	case Render::CubeMapNode::BOTTOM:
		forward = Math::point(0, -1, 0);
		up = Math::point(0, 0, -1);
		break;
	case Render::CubeMapNode::RIGHT:
		forward = Math::point(1, 0, 0);
		up = Math::point(0, 1, 0);
		break;
	case Render::CubeMapNode::LEFT:
		forward = Math::point(-1, 0, 0);
		up = Math::point(0, 1, 0);
		break;
	case Render::CubeMapNode::FRONT:
		forward = Math::point(0, 0, -1);
		up = Math::point(0, 1, 0);
		break;
	case Render::CubeMapNode::BACK:
		forward = Math::point(0, 0, -1);
		up = Math::point(0, 1, 0);
		break;
	default:
		break;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeSampler);
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, this->cubeSampler, 0);
	const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawbuffers[0]);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "Cubemap Framebuffer Status Error!");

	//Render to the given framebuffer / texture
	RenderDevice::Instance()->RenderToTexture(framebuffer, camera);

	//Generate Mipmaps


	
}

GLuint CubeMapNode::GetCubeMap()
{
	return this->cubeSampler;
}

bool CubeMapNode::IsLoaded()
{
	return this->isLoaded;
}

}