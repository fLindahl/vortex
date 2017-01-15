#pragma once
#include "drawpass.h"

namespace Render
{
class FlatGeometryLitPass : public DrawPass
{
public:
	FlatGeometryLitPass();
	~FlatGeometryLitPass();

    void Setup();

    void Execute();

	void UpdateResolution();

	GLuint GetNormalBuffer() { return this->normalBuffer; }

private:

	GLenum drawbuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLuint normalBuffer;

};

}