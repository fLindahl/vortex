/*
	Compute pass that generates a reflection buffer that we can multiply with our specular buffer
*/

#pragma once
#include "framepass.h"

namespace Render
{
class ReflectionPass : public FramePass
{
public:
	ReflectionPass();
	~ReflectionPass();

    void Setup();

    void Execute();

private:
	
	GLuint SSRComputeProgram;
	GLuint reflectionBuffer;
};

}