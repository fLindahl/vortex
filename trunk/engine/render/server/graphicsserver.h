/**
Singleton GraphicsServer

Contains all graphical objects and is used to call their draw functions and more.
*/

#pragma once
#include <unordered_map>
#include "GLFW/glfw3.h"

#include "foundation/util/array.h"

namespace Render
{

class GraphicsServer
{
private:
	GraphicsServer()
	{
	}

public:
	static GraphicsServer* Instance()
	{
		static GraphicsServer instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	GraphicsServer(GraphicsServer const&) = delete;
	void operator=(GraphicsServer const&) = delete;

private:

};

}