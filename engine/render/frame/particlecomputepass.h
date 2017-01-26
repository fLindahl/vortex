#pragma once
#include "framepass.h"
#include "render/particlesystem/particlesystem.h"

namespace Render{ class ShaderObject; }

namespace Render
{
class ParticleComputePass : public FramePass
{
public:
	ParticleComputePass();
	~ParticleComputePass();

	void Setup();

	void Execute();

private:
	Particles::ParticleSystem* particleSystem;
	//std::shared_ptr<ShaderObject> computeShader;
	GLuint particleComputeProgram;

	std::shared_ptr<Render::ShaderObject> sh;

};

}
