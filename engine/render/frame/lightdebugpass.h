#pragma once
#include "drawpass.h"

namespace Render
{
class LightDebugPass : public DrawPass
{
public:
	LightDebugPass();
	~LightDebugPass();

    void Setup();

    void Execute();

	void UpdateResolution();

private:
	GLuint shader;

	GLuint vao[1];
	GLuint vbo[1];
	GLuint ibo[1];

};

}