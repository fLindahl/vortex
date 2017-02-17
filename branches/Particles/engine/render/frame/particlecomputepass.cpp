#include "config.h"
#include "particlecomputepass.h"

#include "render/resources/meshresource.h"
#include "render/server/renderdevice.h"
#include "depthpass.h"
#include "render/properties/graphicsproperty.h"
#include "application/properties/particleemitter.h"
#include "render/server/frameserver.h"
#include "flatgeometrylitpass.h"

#define WORK_GROUP_SIZE 256

namespace Render
{
ParticleComputePass::ParticleComputePass()
{
	
}

ParticleComputePass::~ParticleComputePass()
{}

void ParticleComputePass::Setup()
{
	// Setup light culling compute shader program

	Particles::ParticleSystem::Instance()->GetParticleShaderObject() = Render::ShaderServer::Instance()->LoadShader("defaultUnLitBillboard");

	this->particleComputeProgram = ShaderServer::Instance()->LoadShader("ParticleCompute")->GetProgram();

	FramePass::Setup();
}

void ParticleComputePass::Execute()
{
	

	// Particle compute shader
	glUseProgram(this->particleComputeProgram);

	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(this->particleComputeProgram, "depthMap"), 4);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer());

	glActiveTexture(GL_TEXTURE5);
	glUniform1i(glGetUniformLocation(this->particleComputeProgram, "normalMap"), 5);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer());

	// Bind shader storage buffer objects for the particle buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, Particles::ParticleSystem::Instance()->GetParticleBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, Particles::ParticleSystem::Instance()->GetParticleStartBuffer());
	// Unbind the maps
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDispatchCompute((Particles::ParticleSystem::Instance()->GetParticleArray().Size() / WORK_GROUP_SIZE)+1, 1, 1);

	Particles::ParticleSystem::Instance()->DrawParticleSystem();

	glUseProgram(0);
	
	FramePass::Execute();
}
}

