#pragma once
#include "drawpass.h"

namespace Render
{
class DepthPass : public DrawPass
{
public:
    DepthPass();
    ~DepthPass();

    void Setup();

    void Execute();

	void UpdateResolution();

	// returns the linear depth buffer handle
	GLuint GetLinearDepthBuffer() { return this->linearDepthBuffer; }

	// Returns the depth cone map handle. Do not use unless you know what you're doing!
	// This buffer is computed in an async compute pass and can not be guaranteed to be done when you want to use it!
	GLuint GetDepthConeMap() { return this->depthConeMap; }

private:
	// linearized depthbuffer. Used for various effects such as SSR and particle depth collisions
	GLuint linearDepthBuffer;

	// depth cone map. used for SSR
	GLuint depthConeMap;

	// Program for generating depth cone map from depth map
	GLuint genDepthConeMapComputeProgram;
};

}