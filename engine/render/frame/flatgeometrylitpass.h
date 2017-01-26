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

	//Specular buffer contains roughness in it's alpha channel
	GLuint GetSpecularBuffer() { return this->specularBuffer; }

private:
	GLuint normalBuffer;

	//Specular buffer contains roughness in it's alpha channel
	GLuint specularBuffer;

};

}