#include "config.h"
#include "render/server/frameserver.h"
#include "reflectionpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"

namespace Render
{

ReflectionPass::ReflectionPass()
{

}

ReflectionPass::~ReflectionPass()
{

}

void ReflectionPass::Execute()
{
	FramePass::Execute();
}

void ReflectionPass::Setup()
{
    FramePass::Setup();
}

}