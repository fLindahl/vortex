#pragma once
#include "framepass.h"

namespace Render
{
class DepthPass : public FramePass
{
public:
    DepthPass();
    ~DepthPass();

    void Setup();

    void Execute();

private:

};

}