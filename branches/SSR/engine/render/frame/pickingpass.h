#pragma once
#include "drawpass.h"

namespace Render
{
class ShaderObject;

class PickingPass : public DrawPass
{
public:
	PickingPass();
	~PickingPass();

    void Setup();

    void Execute();

	void UpdateResolution();

private:
	std::shared_ptr<ShaderObject> shader;
	
	GLuint shaderIDLocation;

};

}