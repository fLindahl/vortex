#pragma once
#include "drawpass.h"

namespace Render
{
class ReflectionPass : public DrawPass
{
public:
	ReflectionPass();
	~ReflectionPass();

    void Setup();

    void Execute();

private:

};

}