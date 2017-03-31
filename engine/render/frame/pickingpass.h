#pragma once
#include "drawpass.h"

namespace Render
{
class ShaderObject;

class PickingPass : public DrawPass
{
__DeclareClass(PickingPass);
public:
	PickingPass();
	~PickingPass();

    void Setup();

    void Execute();

	void UpdateResolution();

private:
	
};

}