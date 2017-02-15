#include "config.h"
#include "cubemapnode.h"
#include "render/server/lightserver.h"

namespace Render
{

CubeMapNode::CubeMapNode() :
		isLoaded(false),
		isActive(false),
		resolution({ 128, 128 }),
		mipLevels(6),
		shape(SPHERE),
		innerScale(0.5f,0.5f,0.5f),
		outerScale(1.0f,1.0f,1.0f)
{
	glGenTextures(1, &this->cubeSampler);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeSampler);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	for (size_t i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, this->resolution.x, this->resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	this->isLoaded = true;

	this->proxy = nullptr;
}

CubeMapNode::~CubeMapNode()
{
	this->DeleteCubeMap();
}

void CubeMapNode::DeleteCubeMap()
{
	if (this->isLoaded)
	{
		Deactivate();
		glDeleteTextures(1, &this->cubeSampler);
	}
	this->isLoaded = false;
}

void CubeMapNode::Activate()
{
	if (!this->isActive)
	{
		LightServer::Instance()->AddCubeMap(this->shared_from_this());
		this->isActive = true;
	}
}

void CubeMapNode::Deactivate()
{
	if (this->isActive)
	{
		LightServer::Instance()->RemoveCubeMap(this->shared_from_this());
		this->isActive = false;
	}
}

void CubeMapNode::SetPosition(Math::point pos)
{
	this->position = pos;
}

const Math::point& CubeMapNode::GetPosition() const
{
	return this->position;
}

void CubeMapNode::GenerateCubeMap()
{
#ifdef _DEBUG
	glFinish();
#endif

	double time = glfwGetTime();

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

#ifdef _DEBUG
	glFinish();
	printf("Generate Cubemap: %f seconds elapsed\n", glfwGetTime() - time);
#endif
}

void CubeMapNode::RenderTexture(const GLuint& framebuffer, CubeFace face, Graphics::Camera& camera)
{
	Math::point forward;
	Math::point up;
	Math::point right;

	float rotx;
	float roty;

	switch (face)
	{
	case Render::CubeMapNode::LEFT:
		rotx = -1.57075f;
		roty = 0;
		break;
	case Render::CubeMapNode::RIGHT:
		rotx = 1.57075f;
		roty = 0;
		break;
	case Render::CubeMapNode::BOTTOM:
		rotx = 0;
		roty = 1.57075f;
		break;
	case Render::CubeMapNode::TOP:
		rotx = 0;
		roty = -1.57075f;
		break;
	case Render::CubeMapNode::BACK:
		rotx = 0;
		roty = 0;
		break;
	case Render::CubeMapNode::FRONT:
		rotx = 3.1415f;
		roty = 0;
		break;
	default:
		break;
	}

	Math::mat4 rotation = Math::mat4::multiply(Math::mat4::rotationy(rotx), Math::mat4::rotationx(roty));
	Math::point yaxis = rotation.get_yaxis();
	Math::point zaxis = rotation.get_zaxis();
	camera.SetPosition(this->position);
	camera.LookAt(this->position + zaxis, yaxis);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeSampler);
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, this->cubeSampler, 0);
	const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawbuffers[0]);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "Cubemap Framebuffer Status Error!");

	//Render to the given framebuffer / texture
	RenderDevice::Instance()->RenderToTexture(framebuffer, camera);
}

GLuint CubeMapNode::GetCubeMap()
{
	return this->cubeSampler;
}

bool CubeMapNode::IsLoaded()
{
	return this->isLoaded;
}

void CubeMapNode::CalculateInfluenceWeights(const Math::point& pos)
{
	Math::vector dir = pos - this->position;
	this->NDF = (dir.length() - this->innerScale.x()) / (this->outerScale.x() - this->innerScale.x());
}

}