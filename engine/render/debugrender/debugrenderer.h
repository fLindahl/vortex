#pragma once

//make sure to include all shapes so that we can just include this file if no matter what we want to use
//#include <foundation/util/queue.h>
#include <queue>
#include "shape.h"
#include "line.h"


namespace Debug
{

class DebugRenderer
{
private:
	DebugRenderer();


public:
	static DebugRenderer* Instance()
	{
		static DebugRenderer instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	DebugRenderer(DebugRenderer const&) = delete;
	void operator=(DebugRenderer const&) = delete;

	void Initialize();

	void DrawLine(const Math::point& startPoint, const Math::point& endPoint, const float& lineWidth, const Math::vec4& startColor, const Math::vec4& endColor);

private:
	friend class Render::RenderDevice;

	///Renderers
	RenderLine line;

	void DrawCommands();

	std::queue<RenderCommand*> commandQueue;
};


}