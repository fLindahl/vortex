#include "config.h"
#include "render/server/frameserver.h"
#include "flatgeometrylitpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/server/renderdevice.h"
#include "depthpass.h"

namespace Render
{
FlatGeometryLitPass::FlatGeometryLitPass()
{
    this->frameBufferObject = 0;
    this->buffer = 0;
}

FlatGeometryLitPass::~FlatGeometryLitPass()
{

}

void FlatGeometryLitPass::Execute()
{
	//Just use the regular draw pass at the moment
	this->BindFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawPass::Execute();

    FramePass::Execute();
}

void FlatGeometryLitPass::Setup()
{
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glGenTextures(1, &this->buffer);
	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &this->normalBuffer);
	glBindTexture(GL_TEXTURE_2D, this->normalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &this->specularBuffer);
	glBindTexture(GL_TEXTURE_2D, this->specularBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &this->frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->buffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->normalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->specularBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FrameServer::Instance()->GetDepthPass()->GetBuffer());

	//drawbuffers
	const GLenum drawbuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, &drawbuffers[0]);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "FlatGeometryLit Framebuffer Status Error!");
	//GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FramePass::Setup();
}

void FlatGeometryLitPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);
	
	glBindTexture(GL_TEXTURE_2D, this->normalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->specularBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

}