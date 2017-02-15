#include "config.h"
#include "render/server/frameserver.h"
#include "dynamicunlitpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/server/renderdevice.h"
#include "render/frame/flatgeometrylitpass.h"
#include "depthpass.h"

namespace Render
{
DynamicUnlitPass::DynamicUnlitPass()
{
    this->frameBufferObject = 0;
    this->buffer = 0;
}

DynamicUnlitPass::~DynamicUnlitPass()
{

}

void DynamicUnlitPass::Execute()
{
	this->BindFrameBuffer();

	DrawPass::Execute();

    FramePass::Execute();
}

void DynamicUnlitPass::Setup()
{
	//Uses the original framebuffer from flat geometry lit pass.
	this->frameBufferObject = FrameServer::Instance()->GetFlatGeometryLitPass()->GetFrameBufferObject();
	
	FramePass::Setup();
}

void DynamicUnlitPass::UpdateResolution()
{
	
}

}