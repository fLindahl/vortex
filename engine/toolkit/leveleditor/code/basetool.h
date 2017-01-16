/*------------------/
Class Tools::BaseTool

Baseclass for all tools
/------------------*/
#pragma once
#include "foundation/math/vector.h"
#include "foundation/math/matrix4.h"

namespace Tools
{

enum TransformHandle
{
	NONE,
	XAXIS,
	YAXIS,
	ZAXIS,
	ORIGIN
};

enum ToolType
{
	SELECT,
	TRANSLATE,
	ROTATE,
	SCALE,
	ENTITY,
	BRUSH,
	POLYGONEDIT
};

class BaseTool
{
public:
	BaseTool() {}
	virtual ~BaseTool() {}

	virtual void LeftDown() { this->leftMouse = true; }
	virtual void LeftUp() { this->leftMouse = false; }

	virtual void RightDown() { this->rightMouse = true; }
	virtual void RightUp() { this->rightMouse = false; }

	virtual void MiddleDown() { this->middleMouse = true; }
	virtual void MiddleUp() { this->middleMouse = false; }

	virtual void Drag(const int& deltaX, const int& deltaY) { }

	virtual void Render() { }

	virtual const ToolType& Type() { return this->type; }

	Math::vector FindOrtho(Math::vector& v);

	TransformHandle GetCurrentHandle() { return this->currentHandle; }

	virtual void UpdateTransform(const Math::mat4& transform) { } 
	virtual void UpdateHandlePositions() { }

protected:
	ToolType type;

	TransformHandle currentHandle;

	bool leftMouse;
	bool rightMouse;
	bool middleMouse;

	Math::mat4 initialMatrix;
	Math::mat4 deltaMatrix;
	Math::mat4 startDragMatrix;


};

inline Math::vector BaseTool::FindOrtho(Math::vector& v)
{
	if (0.0 != v.x())
	{
		return Math::vector((-v.y() - v.z()) / v.x(), 1.0, 1.0);
	}
	else if (0.0 != v.y())
	{
		return Math::vector(1.0, (-v.x() - v.z()) / v.y(), 1.0);
	}
	else if (0.0 != v.z())
	{
		return Math::vector(1.0, 1.0, (-v.x() - v.y()) / v.z());
	}
	else
	{
		return Math::vector(0.0, 0.0, 0.0);
	}
}
}