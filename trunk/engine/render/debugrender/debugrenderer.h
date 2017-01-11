#pragma once

//make sure to include all shapes so that we can just include this file if no matter what we want to use
//#include <foundation/util/queue.h>
#include <queue>
#include "shape.h"
#include "line.h"
#include "box.h"


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
	

	///Draws a unit colored cube at position with rotation and scale
	void DrawBox(const Math::vector& position, const Math::quaternion& rotation, const float& scale, const Math::vec4& color, const bool& wireframe = false, const float& lineWidth = 1.0f);
	///Draws a colored box at position with rotation. Size of box will be width (x-axis), height (y-axis), length (z-axis).
	void DrawBox(const Math::vector& position, const Math::quaternion& rotation, const float& width, const float& height, const float& length, const Math::vec4& color, const bool& wireframe = false, const float& lineWidth = 1.0f);
	///Draws a colored box with transform
	void DrawBox(const Math::mat4& transform, const Math::vec4& color, const bool& wireframe = false, const float& lineWidth = 1.0f);
	///Draws a boundingbox
	void DrawBox(const Math::bbox& bbox, const Math::vec4& color, const float& lineWidth = 1.0f);

private:
	friend class Render::RenderDevice;

	///Renderers
	RenderLine line;
	RenderBox box;

	void DrawCommands();

	std::queue<RenderCommand*> commandQueue;
};


}