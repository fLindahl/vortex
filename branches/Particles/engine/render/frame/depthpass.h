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

private:

};

}