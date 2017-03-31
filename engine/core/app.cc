//------------------------------------------------------------------------------
// app.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "app.h"

#include "render/server/renderdevice.h"
#include "render/server/frameserver.h"
#include "render/server/shaderserver.h"
#include "render/server/resourceserver.h"
#include "render/debugrender/debugrenderer.h"

namespace Core
{

//------------------------------------------------------------------------------
/**
*/
App::App() :
	isOpen(false)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
App::~App()
{
	// empty
}


//------------------------------------------------------------------------------
/**
*/
bool
App::Open()
{
	assert(!this->isOpen);
	this->isOpen = true;

	//Init RenderDevice
	Render::RenderDevice::Instance()->Initialize();
	//Setup shaders before framepasses and materials!
	Render::ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.xml");
	//Setup framepasses before materials
	Render::FrameServer::Instance()->SetupFramePasses();
	//Load all materials
	Render::ResourceServer::Instance()->SetupMaterials("resources/materials/default.xml");
	//Init debugrenderer. Always do this AFTER setting up shaders!
	Debug::DebugRenderer::Instance()->Initialize();

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void
App::Run()
{
	// Override in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
App::Close()
{
	assert(this->isOpen);
	this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
App::Exit()
{
	
}

} // namespace Application