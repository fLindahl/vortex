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