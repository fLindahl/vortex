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

private:

};

}