#pragma once
#include "drawpass.h"

namespace Render
{
class DynamicUnlitPass : public DrawPass
{
	__DeclareClass(DynamicUnlitPass);

public:
	DynamicUnlitPass();
	~DynamicUnlitPass();

    void Setup();

    void Execute();

	void UpdateResolution();

private:
	
};

}