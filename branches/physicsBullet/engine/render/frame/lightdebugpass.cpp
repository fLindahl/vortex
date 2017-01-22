#include "config.h"
#include "render/server/frameserver.h"
#include "lightdebugpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/server/shaderserver.h"
#include "render/server/lightserver.h"

namespace Render
{

LightDebugPass::LightDebugPass()
{

}

LightDebugPass::~LightDebugPass()
{

}

void LightDebugPass::Execute()
{
	// Bind the depth map's frame buffer and draw the depth map to it
	this->BindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT);

	auto shader = ShaderServer::Instance()->LoadShader("debugLights");
	glUseProgram(shader->GetProgram());
	
	shader->setModelMatrix(Graphics::MainCamera::Instance()->getInvProjection());
	
	glDisable(GL_CULL_FACE);

	GLuint matLoc = glGetUniformLocation(shader->GetProgram(), "totalLightCount");
	glUniform1i(matLoc, (GLint)LightServer::Instance()->GetNumPointLights());

	glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);



	glUseProgram(0);

	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    FramePass::Execute();
}

void LightDebugPass::Setup()
{
    glGenFramebuffers(1, &this->frameBufferObject);    
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glGenTextures(1, &this->buffer);
	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
	
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->buffer, 0);    
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "Lightdebug Framebuffer Status Error!");

	//Setup a fullscreen quad
	const GLfloat mesh[] =
	{
		1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,
		-1.0, -1.0, -1.0,
		1.0, 1.0, -1.0
	};

	const int indices[] =
	{
		//Triangles
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &mesh[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, NULL);

	glGenBuffers(1, this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    FramePass::Setup();
}

void LightDebugPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

}