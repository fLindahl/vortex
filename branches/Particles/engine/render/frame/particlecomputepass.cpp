#include "config.h"
#include "particlecomputepass.h"
#include "render/server/frameserver.h"
#include "render/resources/meshresource.h"
#include "render/server/renderdevice.h"
#include "depthpass.h"
#include "flatgeometrylitpass.h"
#include "render/properties/graphicsproperty.h"
#include "application/properties/particleemitter.h"

#define WORK_GROUP_SIZE 128

namespace Render
{
ParticleComputePass::ParticleComputePass()
{
	particleSystem = Particles::ParticleSystem::Instance();
}

ParticleComputePass::~ParticleComputePass()
{}

void ParticleComputePass::Setup()
{
	// Setup light culling compute shader program

	sh = Render::ShaderServer::Instance()->LoadShader("defaultUnLitBillboard");

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
	//glActiveTexture(GL_TEXTURE4);
	//glUniform1i(glGetUniformLocation(this->particleComputeProgram, "depthMap"), 4);
	//Get and bind Depth map
	//glBindTexture(GL_TEXTURE_2D, Render::FrameServer::Instance()->GetDepthPass()->GetTexture());

	// Bind shader storage buffer objects for the particle buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, Particles::ParticleSystem::Instance()->GetParticleBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, Particles::ParticleSystem::Instance()->GetParticleStartBuffer());

	glDispatchCompute((Particles::ParticleSystem::Instance()->GetParticleArray().Size() / WORK_GROUP_SIZE), 1, 1);

	Render::FrameServer::Instance()->GetFlatGeometryLitPass()->BindFrameBuffer();
	//Because we're using the old depthbuffer from the depth prepass, we need to allow equal depth values, or clear the depth buffer. clearing would mean redundant work however.
	//glDepthFunc(GL_LEQUAL);
	glUseProgram(sh->GetProgram());
	std::shared_ptr<Render::MeshResource> mesh = Particles::ParticleSystem::Instance()->GetMesh();


	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mesh->Bind();
	for (size_t i = 0; i < Particles::ParticleSystem::Instance()->GetParticleEmitters().Size(); i++)
	{
		Particles::ParticleSystem::Instance()->GetParticleEmitters()[i]->GetEmitterTexture()->BindTexture(0);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->getNumIndices(), GL_UNSIGNED_INT, NULL, Particles::ParticleSystem::Instance()->GetParticleEmitters()[i]->GetNumberOfParticles());	
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	mesh->Unbind();
	//glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	//glDepthFunc(GL_LESS);
	FramePass::Execute();
}
}

