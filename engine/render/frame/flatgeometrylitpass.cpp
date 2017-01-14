#include "config.h"
#include "render/server/frameserver.h"
#include "flatgeometrylitpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"

namespace Render
{
FlatGeometryLitPass::FlatGeometryLitPass()
{
    this->frameBufferObject = 0;
    this->buffer = 0;
}

FlatGeometryLitPass::~FlatGeometryLitPass()
{

}

void FlatGeometryLitPass::Execute()
{
	//Just use the regular draw pass at the moment
	DrawPass::Execute();

    FramePass::Execute();
}
void FlatGeometryLitPass::Setup()
{
	FramePass::Setup();
}

}