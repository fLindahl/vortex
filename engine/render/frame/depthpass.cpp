#include "config.h"
#include "render/server/frameserver.h"
#include "depthpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "render/server/lightserver.h"

namespace Render
{

DepthPass::DepthPass()
{

}

DepthPass::~DepthPass()
{

}

void DepthPass::Execute()
{
	// Bind the depth map's frame buffer and draw the depth map to it
	this->BindFrameBuffer();
	glClear(GL_DEPTH_BUFFER_BIT);

    GLuint currentProgram = 0;

    for (Material* material : this->materials)
    {
        auto shader = material->GetShader(this->name);

        if (shader->GetProgram() != currentProgram)
        {
            currentProgram = shader->GetProgram();
            glUseProgram(currentProgram);
        }

        //TODO: Renderstates?
        for (auto surface : material->SurfaceList())
        {
            for (auto modelNode : surface->GetModelNodes())
            {
                //Bind mesh
                //TODO: We should probably check and make sure we don't bind these more than once
                modelNode->modelInstance->GetMesh()->Bind();

				for (GraphicsProperty* graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
                {
                    shader->setModelMatrix(graphicsProperty->getModelMatrix());
					modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
                }

				modelNode->modelInstance->GetMesh()->Unbind();
            }
        }
    }

	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);

	/*

	// generate cone map depth buffer compute shader
	glUseProgram(this->genDepthConeMapComputeProgram);

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(genDepthConeMapComputeProgram, "depthMap"), 4);

	glBindTexture(GL_TEXTURE_2D, this->buffer);

	const GLint location = glGetUniformLocation(genDepthConeMapComputeProgram, "coneMap");
	if (location == -1){
		printf("Could not locate uniform location for texture in genDepthConeMapComputeProgram");
	}

	glUniform1i(location, 0);
	glBindImageTexture(0, this->depthConeMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	 
	// Dispatch the compute shader, using the workgroup values calculated earlier
	// TODO: This shouldnt be lightserver work groups.
	glDispatchCompute(LightServer::Instance()->GetWorkGroupsX(), LightServer::Instance()->GetWorkGroupsY(), 1);

	// Unbind the depth map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	*/

    FramePass::Execute();
}

void DepthPass::Setup()
{
    glGenFramebuffers(1, &this->frameBufferObject);
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glGenTextures(1, &this->buffer);
    glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glGenTextures(1, &this->linearDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//glGenerateMipmap(GL_TEXTURE_2D);

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->buffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->linearDepthBuffer, 0);
    
	const GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawbuffers[0]);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	_assert(e == GL_FRAMEBUFFER_COMPLETE, "Depth Framebuffer Status Error!");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//Setup compute pass for generating cone map
	/*
	this->genDepthConeMapComputeProgram = glCreateProgram();
	const char filepath[] = "resources/shaders/compute/generaterelaxedconemap.comp";
	glAttachShader(this->genDepthConeMapComputeProgram, ShaderServer::Instance()->LoadComputeShader(filepath));
	glLinkProgram(this->genDepthConeMapComputeProgram);
	GLint shaderLogSize;
	glGetProgramiv(this->genDepthConeMapComputeProgram, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->genDepthConeMapComputeProgram, shaderLogSize, NULL, buf);
		printf("[COMPUTE LINK ERROR]: %s", buf);
		delete[] buf;
	}

	glGenTextures(1, &this->depthConeMap);
	glBindTexture(GL_TEXTURE_2D, this->depthConeMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/
    FramePass::Setup();
}

void DepthPass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, newRes.x, newRes.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, newRes.x, newRes.y, 0, GL_RED, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->depthConeMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, newRes.x, newRes.y, 0, GL_RED, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

}