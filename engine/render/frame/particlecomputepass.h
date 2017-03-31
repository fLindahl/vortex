#pragma once
#include "framepass.h"
#include "render/particlesystem/particlesystem.h"


namespace Render
{
class ParticleComputePass : public FramePass
{
__DeclareClass(ParticleComputePass);

public:
	ParticleComputePass();
	~ParticleComputePass();

	void Setup();

	void Execute();

private:

	//Ptr<ShaderObject> computeShader;
	GLuint particleComputeProgram;

	

};

}
