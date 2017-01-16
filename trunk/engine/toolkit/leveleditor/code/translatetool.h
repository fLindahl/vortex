/*------------------/
Class Tools::TranslateTool

Used for translating selected objects
/------------------*/

#pragma once
#include "basetool.h"
#include "foundation/math/plane.h"
#include "foundation/math/vector.h"
#include "foundation/math/point.h"
#include "foundation/math/line.h"

namespace Render
{
	class GraphicsProperty;
}

namespace Tools
{
	class TranslateTool : public BaseTool
	{
	public:
		TranslateTool();
		~TranslateTool();

		TransformHandle GetMouseHandle(const Math::line& worldMouseRay);
		void LeftDown();
		void LeftUp();

		void Drag(const int& deltaX, const int& deltaY);
		void Render();
		void UpdateHandlePositions();
		void UpdateTransform(const Math::mat4& transform);

	private:
		Math::point xAxis;
		Math::point yAxis;
		Math::point zAxis;
		Math::point origin;
		Math::plane viewPlane;

		//Used to scale object depending on distance to camera so that it's always the same size.
		float snapOffset;
		float handleScale;
		float handleDistance;
		float dragPlaneOffset;
		Math::vector dragStartMouseRayOffset;
		Math::vector dragStart;
		Math::vector lockedDragStart;

		bool relativeMode;
		bool axisLockingMode;
		bool freeModeRequested;
		bool snapMode;
	};
}