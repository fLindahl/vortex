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

	GLuint normalBuffer;

};

}