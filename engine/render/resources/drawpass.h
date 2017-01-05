#pragma once
#include "framepass.h"

namespace Render
{
class DrawPass : public FramePass
{
public:
    DrawPass();
    ~DrawPass();

    void Setup();

    void Execute();

private:

};

}