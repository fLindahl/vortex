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

	GLuint GetLinearDepthBuffer() { return this->linearDepthBuffer; }

private:
	// linearized depthbuffer. Used for various effects such as SSR and particle depth collisions
	GLuint linearDepthBuffer;
};

}