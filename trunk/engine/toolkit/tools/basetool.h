/*------------------/
Class Tools::BaseTool

Baseclass for all tools
/------------------*/
#pragma once

namespace Tools
{
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

		virtual void LeftDown() {}
		virtual void LeftUp() {}

		virtual void RightDown() {}
		virtual void RightUp() {}

		virtual void MiddleDown() {}
		virtual void MiddleUp() {}

		virtual void Drag(const int& deltaX, const int& deltaY) {}

		virtual const ToolType& Type() { return this->type; }

	protected:
		ToolType type;

		bool leftMouse;
		bool rightMouse;
		bool middleMouse;


	};
}