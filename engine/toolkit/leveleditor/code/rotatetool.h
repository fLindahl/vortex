/*------------------/
Class Tools::RotateTool

Used for rotating objects
/------------------*/

#pragma once
#include "basetool.h"
#include "foundation/math/line.h"

namespace Tools
{
	class RotateTool : public BaseTool
	{
	public:
		RotateTool();
		~RotateTool();

		void LeftDown();
		void LeftUp();
		void Drag();

		void Render();

		TransformHandle GetMouseHandle(const Math::line& worldMouseRay);
		void UpdateHandlePositions();
		void UpdateTransform(const Math::mat4& transform) {}
		
		/// decompose matrix into angles in all axis
		void GetAnglesFromMat(const Math::mat4& i_m, float& x, float& y, float& z);

	private:

		/// Helper method to get the closest intersection point of given line with given sphere
		static Math::vector GetSphereIntersectionPoint(const Math::line& ray, const Math::vector& location, float radius);

		/// generate a vector used to compute rotation delta in drag mode.
		Math::vector ComputeDragVector(const Math::line& ray, TransformHandle mode);

		/// rotate vector in a circle using an axis ad pivot
		void RotateVector(Math::vector &i_v, Math::vector &axis, float angle);
		/// calculate plane intersection boolean between two planes and return the intersecting line
		bool PlaneIntersect(Math::plane &p1, Math::plane &p2, Math::line& l);

		Math::mat4 lastStartDragDeltaMatrix;

		Math::vector xAxis;             // handle axes
		Math::vector yAxis;
		Math::vector zAxis;
		Math::vector viewAxis;
		Math::point origin;

		float handleScale;
		float outerCircleScale;
		Math::vector startDragOrientation;

		Math::vector xDragStart;
		Math::vector yDragStart;
		Math::vector zDragStart;

		float handleDistance;

		Math::plane xPlane;              // planes orthogonal to the handle axes
		Math::plane yPlane;
		Math::plane zPlane;
		Math::plane viewPlane;

	};
}