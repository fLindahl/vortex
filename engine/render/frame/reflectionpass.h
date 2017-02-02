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

	void UpdateResolution();

	GLuint GetReflectionBuffer() { return this->reflectionBuffer; }

private:

	struct UniformBlock
	{
		GLfloat zThickness;
		GLfloat stride;
		GLfloat jitter;
		GLfloat maxSteps;
		GLfloat maxDistance;
		GLuint workGroups[2];
		GLfloat padding; //TODO: padding might not be needed here
	} uniformBlock;
	
	//Uniform Buffer Object
	GLuint ubo[1];

	GLuint SSRComputeProgram;
	GLuint reflectionBuffer;
};

}