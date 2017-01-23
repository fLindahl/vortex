#pragma once
#include "framepass.h"
#include "render/particlesystem/particlesystem.h"

namespace Particles
{
class ParticleComputePass : public Render::FramePass
{
public:
	ParticleComputePass();
	~ParticleComputePass();

	void Setup();

	void Execute();

private:
	ParticleSystem* particleSystem;
	//std::shared_ptr<ShaderObject> computeShader;
	GLuint particleComputeProgram;

};

}
