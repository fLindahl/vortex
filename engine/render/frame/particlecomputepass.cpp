#include "config.h"
#include "particlecomputepass.h"
#include "render/resources/meshresource.h"
#include "render/server/renderdevice.h"

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

}
}

