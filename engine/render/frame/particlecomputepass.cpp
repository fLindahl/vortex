#include "config.h"
#include "particlecomputepass.h"
#include "render/server/frameserver.h"
#include "render/resources/meshresource.h"
#include "render/server/renderdevice.h"
#include "depthpass.h"

namespace Particles
{
ParticleComputePass::ParticleComputePass()
{
	particleSystem = ParticleSystem::Instance();
}

ParticleComputePass::~ParticleComputePass()
{}

void ParticleComputePass::Setup()
{
	// Setup light culling compute shader program
	this->particleComputeProgram = glCreateProgram();
	const char filepath[] = "resources/shaders/compute/particlecalc.comp";
	glAttachShader(this->particleComputeProgram, Render::ShaderServer::Instance()->LoadComputeShader(filepath));
	glLinkProgram(this->particleComputeProgram);
	GLint shaderLogSize;
	glGetProgramiv(this->particleComputeProgram, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->particleComputeProgram, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}

	FramePass::Setup();
}

void ParticleComputePass::Execute()
{
	// Light culling compute shader
	glUseProgram(this->particleComputeProgram);

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(this->particleComputeProgram, "depthMap"), 4);
	//Get and bind Depth map
	glBindTexture(GL_TEXTURE_2D, Render::FrameServer::Instance()->GetDepthPass()->GetTexture());
}
}

