#pragma once
#include "framepass.h"
#include "render/particlesystem/particlesystem.h"


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

	//std::shared_ptr<ShaderObject> computeShader;
	GLuint particleComputeProgram;

	

};

}
