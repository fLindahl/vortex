#include "config.h"
#include "particlesystem.h"
#include "render/server/resourceserver.h"
#include "application/properties/particleemitter.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/frameserver.h"
#include "render/frame/flatgeometrylitpass.h"
#include "foundation/math/math.h"

namespace Particles
{


ParticleSystem::ParticleSystem() : particleBuffer(0)
{
	glGenBuffers(1, &particleBuffer);
	glGenBuffers(1, &particleStartBuffer);
	mesh = Render::ResourceServer::Instance()->LoadMesh("resources/meshes/quad_tris.obj");
	sh = Render::ShaderServer::Instance()->LoadShader("defaultUnLitBillboard");
}

ParticleSystem::~ParticleSystem()
{
}

EmitterBuffer ParticleSystem::GetEmitterBuffer(index_t bufferSize, Property::ParticleEmitter& owner)
{

	emitters.Insert(emitters.Size(), &owner);
	//Create a "buffer" for an emitter
	EmitterBuffer buf;
	buf.startIndex = particleArray.Size();
	particleArray.Reserve(bufferSize);
	particleArray.Fill(buf.startIndex, bufferSize, owner.GetState());
	buf.endIndex = particleArray.Size();
	buf.arr = &particleArray;
	buf.startArr = &particleStartSettings;

	owner.GetNumberOfParticles() = buf.endIndex - buf.startIndex;

	index_t numPart = particleArray.Size();
	for (int i = buf.startIndex; i < buf.endIndex; i++)
	{
		Math::vec4 pos(0.0f, 0.0f, 0.0f, 1.0f);
		RandomPointInSphere(1.0f, pos);
		particleArray[i].accLife[3] = Math::randFloat(0.0f, 2.5f);
		particleArray[i].accLife[2] =  Math::randFloat(-0.5f, 1.0f);
		particleArray[i].accLife[1] =  Math::randFloat(-9.82f, -7.82f);
		particleArray[i].accLife[0] =  Math::randFloat(-0.5f, 1.0f);
		particleArray[i].pos = owner.GetModelMatrix().get_position();
		particleArray[i].vel = Math::vec4::normalize(Math::mat4::transform(pos, owner.GetModelMatrix()) - particleArray[i].pos) * Math::randFloat(1.0f, 2.5f);
		particleArray[i].vel[3] = 1.0f;
	}

	particleStartSettings = particleArray;
	for (int i = buf.startIndex; i < buf.endIndex; i++)
	{
		particleStartSettings[i].accLife[3] = Math::randFloat(0.5f, 2.5f);
	}


	// Bind particle buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleArray[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleStartBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleStartSettings[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return buf;
}

void ParticleSystem::GetEmitterBuffer(index_t bufferSize, std::shared_ptr<Property::ParticleEmitter> owner, EmitterBuffer& eBuff)
{
	Util::Array<ParticleState> arr;
	arr.Reserve(particleArray.Size() - (eBuff.endIndex - eBuff.startIndex) + bufferSize);
	for (int i = 0; i < particleArray.Size(); i++)
	{
		if (i >= eBuff.startIndex && i <= eBuff.endIndex)
			continue;

		arr.Insert(i, particleArray[i]);
	}

	//Create a "buffer" for an emitter
	EmitterBuffer buf;
	buf.startIndex = arr.Size();
	arr.Fill(buf.startIndex, bufferSize, owner->GetState());
	buf.endIndex = arr.Size();
	particleArray = arr;
	buf.arr = &particleArray;
	particleStartSettings = particleArray;
	buf.startArr = &particleStartSettings;
	
	eBuff = buf;
	owner->GetNumberOfParticles() = buf.endIndex - buf.startIndex;
	float x, y;
	for (int i = buf.startIndex; i < buf.endIndex; i++)
	{
		Math::vec4 pos(0.0f, 0.0f, 0.0f, 1.0f);
		RandomPointInSphere(1.0f, pos);
		particleArray[i].accLife[3] = Math::randFloat(0.0f, 2.5f);
		particleArray[i].accLife[2] = Math::randFloat(-0.5f, 1.0f);
		particleArray[i].accLife[1] = Math::randFloat(-9.82f, -7.82f);
		particleArray[i].accLife[0] = Math::randFloat(-0.5f, 1.0f);
		particleArray[i].pos = owner->GetModelMatrix().get_position();
		particleArray[i].vel = Math::vec4::normalize(Math::mat4::transform(pos, owner->GetModelMatrix()) - particleArray[i].pos) * Math::randFloat(1.0f, 2.5f);
		particleArray[i].vel[3] = 1.0f;
	}

	particleStartSettings = particleArray;
	for (int i = buf.startIndex; i < buf.endIndex; i++)
	{
		particleStartSettings[i].accLife[3] = Math::randFloat(0.5f, 2.5f);
	}

	index_t numPart = particleArray.Size();
	

	// Bind particle buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleArray[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->particleStartBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPart * sizeof(ParticleState), &this->particleStartSettings[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	

}

void ParticleSystem::DrawParticleSystem()
{
	Render::FrameServer::Instance()->GetFlatGeometryLitPass()->BindFrameBuffer();
	//Because we're using the old depthbuffer from the depth prepass, we need to allow equal depth values, or clear the depth buffer. clearing would mean redundant work however.
	//glDepthFunc(GL_LEQUAL);
	glUseProgram(sh->GetProgram());

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->mesh->Bind();
	for (size_t i = 0; i < GetParticleEmitters().Size(); i++)
	{
		GetParticleEmitters()[i]->GetEmitterTexture()->BindTexture(0);
		GetParticleEmitters()[i]->BindUniformBuffer();
		GLuint num = GetParticleEmitters()[i]->GetNumberOfParticles();
		glDrawElementsInstanced(GL_TRIANGLES, this->mesh->getNumIndices(), GL_UNSIGNED_INT, NULL, num);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	this->mesh->Unbind();
	//glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDepthFunc(GL_LESS);
	glUseProgram(0);
}

void ParticleSystem::UpdateParticlePosition(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GetParticleStartBuffer());
	GLbitfield bufferMask = GL_MAP_WRITE_BIT;
	ParticleState* particles = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, owner->GetEmitterBuffer().startIndex * sizeof(ParticleState), (owner->GetEmitterBuffer().endIndex - owner->GetEmitterBuffer().startIndex) * sizeof(ParticleState), bufferMask);

	if (particles == NULL)
	{
		GLint size;
		glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
		printf("Size is: %i", size / sizeof(ParticleState));
		printf("Error: %i", glGetError());
	}
	
	if (random)
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			
			particles[i].pos = Math::vec4(Math::randFloat(min[0], max[0]), Math::randFloat(min[1], max[1]), Math::randFloat(min[2], max[2]), 1.0f);
		}
	}
	else
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].pos = min;
		}
	}
	owner->GetState().pos = min;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void ParticleSystem::UpdateParticleVelocity(std::shared_ptr<Property::ParticleEmitter> owner, float min, float max, float radius, bool random)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GetParticleStartBuffer());
	GLbitfield bufferMask = GL_MAP_WRITE_BIT;
	ParticleState* particles = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, owner->GetEmitterBuffer().startIndex * sizeof(ParticleState), (owner->GetEmitterBuffer().endIndex - owner->GetEmitterBuffer().startIndex) * sizeof(ParticleState), bufferMask);
	
	if (particles == NULL)
	{
		GLint size;
		glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
		printf("Size is: %i", size/sizeof(ParticleState));
		printf("Error: %i", glGetError());
	}
	float x, y;
	if (random)
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			Math::vec4 pos(0.0f, 2.0f, 0.0f, 1.0f);
			Math::RandomPointInCircle(radius, x, y);
			pos.set_x(x);
			pos.set_z(y);
			particles[i].vel = Math::vec4::normalize(Math::mat4::transform(pos, owner->GetModelMatrix()) - particles[i].pos)*Math::randFloat(min, max);
			particles[i].vel[3] = 1.0f;
		}
	}
	else
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			Math::vec4 pos(0.0f, 2.0f, 0.0f, 1.0f);
			Math::RandomPointInCircle(radius, x, y);
			pos.set_x(x);
			pos.set_z(y);
			particles[i].vel = Math::vec4::normalize(Math::mat4::transform(pos, owner->GetModelMatrix()) - particles[i].pos)*min;
			particles[i].vel[3] = 1.0f;
		}
	}

	owner->GetState().vel = particles[0].vel;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleSystem::UpdateParticleRotation(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GetParticleStartBuffer());
	GLbitfield bufferMask = GL_MAP_WRITE_BIT;
	ParticleState* particles = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, owner->GetEmitterBuffer().startIndex * sizeof(ParticleState), (owner->GetEmitterBuffer().endIndex - owner->GetEmitterBuffer().startIndex) * sizeof(ParticleState), bufferMask);

	if (particles == NULL)
	{
		GLint size;
		glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
		printf("Size is: %i", size / sizeof(ParticleState));
		printf("Error: %i", glGetError());
	}

	if (random)
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].rot = Math::vec4(Math::randFloat(min[0], max[0]), Math::randFloat(min[1], max[1]), Math::randFloat(min[2], max[2]), 1.0f);
		}
	}
	else
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].rot = min;
		}
	}
	owner->GetState().rot = min;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void ParticleSystem::UpdateParticleAcceleration(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GetParticleStartBuffer());
	GLbitfield bufferMask = GL_MAP_WRITE_BIT;
	ParticleState* particles = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, owner->GetEmitterBuffer().startIndex * sizeof(ParticleState), (owner->GetEmitterBuffer().endIndex - owner->GetEmitterBuffer().startIndex) * sizeof(ParticleState), bufferMask);

	if (particles == NULL)
	{
		GLint size;
		glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
		printf("Size is: %i", size / sizeof(ParticleState));
		printf("Error: %i", glGetError());
	}

	if (random)
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].accLife[0] = Math::randFloat(min[0], max[0]);
			particles[i].accLife[1] = Math::randFloat(min[1], max[1]);
			particles[i].accLife[2] = Math::randFloat(min[2], max[2]);
		}		
	}
	else
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].accLife[0] = min[0];
			particles[i].accLife[1] = min[1];
			particles[i].accLife[2] = min[2];
		}
	}
	owner->GetState().accLife[0] = min[0];
	owner->GetState().accLife[1] = min[1];
	owner->GetState().accLife[2] = min[2];
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void ParticleSystem::UpdateParticleLifetime(std::shared_ptr<Property::ParticleEmitter> owner, float min, float max, bool random)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GetParticleStartBuffer());
	GLbitfield bufferMask = GL_MAP_WRITE_BIT;
	ParticleState* particles = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, owner->GetEmitterBuffer().startIndex * sizeof(ParticleState), (owner->GetEmitterBuffer().endIndex - owner->GetEmitterBuffer().startIndex) * sizeof(ParticleState), bufferMask);

	if (particles == NULL)
	{
		GLint size;
		glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
		printf("Size is: %i", size / sizeof(ParticleState));
		printf("Error: %i", glGetError());
	}

	if (random)
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].accLife[3] = Math::randFloat(min, max);
		}
	}
	else
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].accLife[3] = min;
		}
	}
	owner->GetState().accLife[3] = min;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void ParticleSystem::UpdateParticleColor(std::shared_ptr<Property::ParticleEmitter> owner, Math::vec4 min, Math::vec4 max, bool random)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GetParticleStartBuffer());
	GLbitfield bufferMask = GL_MAP_WRITE_BIT;
	ParticleState* particles = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, owner->GetEmitterBuffer().startIndex * sizeof(ParticleState), (owner->GetEmitterBuffer().endIndex - owner->GetEmitterBuffer().startIndex) * sizeof(ParticleState), bufferMask);

	if (particles == NULL)
	{
		GLint size;
		glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
		printf("Size is: %i", size / sizeof(ParticleState));
		printf("Error: %i", glGetError());
	}

	if (random)
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].color = Math::vec4::lerp(min,max,Math::randFloat());
		}
	}
	else
	{
		for (int i = 0; i < owner->GetNumberOfParticles(); ++i)
		{
			particles[i].color = min;
		}
	}
	owner->GetState().color = min;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	
}

void ParticleSystem::UpdateParticleSize(std::shared_ptr<Property::ParticleEmitter> owner, float start, float end)
{
	owner->GetRenderBuffer().startSize = Math::vec4(start);
	owner->GetRenderBuffer().endSize = Math::vec4(end);
	glBindBuffer(GL_UNIFORM_BUFFER, owner->GetUniformBuffer()[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, owner->GetUniformBuffer()[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(owner->GetRenderBuffer()), &owner->GetRenderBuffer(), GL_STATIC_DRAW);
}

std::shared_ptr<Render::ShaderObject> ParticleSystem::GetParticleShaderObject()
{
	return this->sh;
}

}



