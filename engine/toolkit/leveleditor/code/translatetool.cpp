#include "translatetool.h"
#include "foundation/math/line.h"
#include "imgui.h"
#include "application/basegamefeature/managers/envmanager.h"
#include "render/debugrender/debugrenderer.h"
#include "foundation/math/math.h"
namespace Tools
{

TranslateTool::TranslateTool() : 
			handleDistance(5.0f),
			relativeMode(false),
			axisLockingMode(false),
			dragPlaneOffset(0.0f),
			freeModeRequested(false),
			dragStartMouseRayOffset(0.0f, 0.0f, 0.0f),
			snapOffset(1.0f),
			snapMode(false)
{
	type = ToolType::TRANSLATE;
	currentHandle = TransformHandle::NONE;
	this->xAxis = Math::point(1.0f, 0.0f, 0.0f);
	this->yAxis = Math::point(0.0f, 1.0f, 0.0f);
	this->zAxis = Math::point(0.0f, 0.0f, 1.0f);
}

TranslateTool::~TranslateTool()
{
}
	
TransformHandle TranslateTool::GetMouseHandle(const Math::line& worldMouseRay)
{
	// Check distance to each of the handles of the feature.
	// If the distance to a handle is close enough, activate drag mode.
	float distance;
	float activationDistance = 1.0f * this->handleScale;

	// needed for distance calculation of x,y,z handles
	Math::point rayPoint, handlePoint;
	Math::line axis;
	float axis_t;
	float nearestHandle = 10000000.0f;

	TransformHandle modes[] = { XAXIS, YAXIS, ZAXIS };
	Math::vector handles[] = { this->xAxis, this->yAxis, this->zAxis };
	Math::vector handlePointSize[] = { Math::vector(this->handleScale, 0, 0), Math::vector(0, this->handleScale, 0), Math::vector(0, 0, this->handleScale) };
	TransformHandle returnValue = NONE;

	// if we're not above the origin handle, we proceed to check the other handles
	for (index_t i = 0; i < 3; ++i)
	{
		// check all handles
		axis.set(this->origin + handlePointSize[i] * 0.5f, handles[i] + handlePointSize[i]);

		worldMouseRay.IntersectLine(axis, rayPoint, handlePoint);
		axis_t = axis.closestpoint(rayPoint);
		distance = axis.distance(rayPoint);
		if (distance < activationDistance && axis_t < 1 && axis_t > 0 && distance < nearestHandle)
		{
			nearestHandle = distance;
			returnValue = modes[i];
		}
	}

	// check origin handle
	distance = worldMouseRay.distance(this->origin);
	if (distance < activationDistance)
	{
		returnValue = ORIGIN;
	}

	return returnValue;
}

void TranslateTool::LeftDown()
{
	if (!this->leftMouse)
	{
		//Start Dragging
		BaseTool::LeftDown();
		this->startDragMatrix = Math::mat4::identity();
		this->startDragMatrix = Math::mat4::multiply(this->startDragMatrix, this->initialMatrix);
		this->startDragMatrix = Math::mat4::multiply(this->startDragMatrix, this->deltaMatrix);

		Math::point rayPoint, handlePoint;

		// get mouse position on screen
		ImVec2 dockPos = ImGui::GetWindowPos();
		ImGuiStyle& style = ImGui::GetStyle();
		dockPos.x += style.WindowPadding.x;
		dockPos.y += style.WindowPadding.y;
		ImVec2 mousePos = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);

		ImVec2 dockSize = ImGui::GetWindowSize();

		// get mouse ray
		const float rayLength = 5000.0f;
		Math::line worldMouseRay = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mousePos.x, mousePos.y, rayLength, dockSize.x, dockSize.y);

		this->currentHandle = this->GetMouseHandle(worldMouseRay);

		// check origin handle
		if (this->currentHandle == ORIGIN /*|| this->isInFreeDragMode*/)
		{
			Math::line centerViewLine = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(0.5, 0.5, 1, dockSize.x, dockSize.y);
			Math::vector viewVector = centerViewLine.vec();
			Math::vector v1 = FindOrtho(viewVector);
			Math::vector v2 = Math::vector::cross3(viewVector, v1);

			Math::plane translatePlane(this->origin, this->origin + v1, this->origin + v2);
			Math::vec4 t;
			translatePlane.intersectline(worldMouseRay.start(), worldMouseRay.end(), t);
			this->dragStartMouseRayOffset = (Math::vec4(this->origin) - t);
			return;
		}

		Math::vector axis;
		Math::mat4 cameraTrans = Graphics::MainCamera::Instance()->getInvView();
		Math::vector zaxis = cameraTrans.get_zaxis();
		Math::vector lockedAxis = zaxis;

		// determine which handle should be used, then adjust the zaxis to not point in said axis direction
		if (this->currentHandle == XAXIS)
		{
			// set drag plane offset
			this->dragPlaneOffset = this->handleDistance * this->handleScale;
			worldMouseRay.IntersectLine(Math::line(this->origin, this->xAxis + Math::vector(this->handleScale, 0, 0)), rayPoint, handlePoint);
			this->dragStartMouseRayOffset = (rayPoint - this->xAxis);
			zaxis.set_x(0);
			lockedAxis.set_y(0);
			lockedAxis.set_z(0);
			axis = zaxis;
		}
		else if (this->currentHandle == YAXIS)
		{
			// set drag plane offset
			this->dragPlaneOffset = this->handleDistance * this->handleScale;
			worldMouseRay.IntersectLine(Math::line(this->origin, this->yAxis + Math::vector(0, this->handleScale, 0)), rayPoint, handlePoint);
			this->dragStartMouseRayOffset = (rayPoint - this->yAxis);
			zaxis.set_y(0);
			lockedAxis.set_x(0);
			lockedAxis.set_z(0);
			axis = zaxis;
		}
		else if (this->currentHandle == ZAXIS)
		{
			// set drag plane offset
			this->dragPlaneOffset = this->handleDistance * this->handleScale;
			worldMouseRay.IntersectLine(Math::line(this->origin, this->zAxis + Math::vector(0, 0, this->handleScale)), rayPoint, handlePoint);
			this->dragStartMouseRayOffset = (rayPoint - this->zAxis);
			zaxis.set_z(0);
			lockedAxis.set_x(0);
			lockedAxis.set_y(0);
			axis = zaxis;
		}
		else
		{
			return;
		}

		Math::point axispoint;

		// if we are in relative mode, we must project the dragstart back to the relative axis
		if (this->relativeMode)
		{
			Math::plane viewPlane = this->viewPlane;
			viewPlane = Math::plane::normalize(viewPlane);

			viewPlane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);
			this->dragStart = axispoint - Math::point(this->initialMatrix.get_position());

			Math::mat4 rot = this->initialMatrix;
			rot.set_position(Math::point(0, 0, 0));
			Math::mat4 localForward = Math::mat4::lookatlh(this->initialMatrix.get_position(), Math::point(this->initialMatrix.get_position() + rot.get_zaxis()), rot.get_yaxis());

			if (this->currentHandle == XAXIS)
			{
				Math::vector relativeAxis = Math::mat4::transform(Math::vector(1, 0, 0), localForward);
				this->dragStart = relativeAxis * (Math::vec4::dot3(this->dragStart, relativeAxis) / Math::vec4::dot3(relativeAxis, relativeAxis));
			}
			else if (this->currentHandle == YAXIS)
			{
				Math::vector relativeAxis = Math::mat4::transform(Math::vector(0, 1, 0), localForward);
				this->dragStart = relativeAxis * (Math::vec4::dot3(this->dragStart, relativeAxis) / Math::vec4::dot3(relativeAxis, relativeAxis));
			}
			else if (this->currentHandle == ZAXIS)
			{
				Math::vector relativeAxis = Math::mat4::transform(Math::vector(0, 0, 1), localForward);
				this->dragStart = relativeAxis * (Math::vec4::dot3(this->dragStart, relativeAxis) / Math::vec4::dot3(relativeAxis, relativeAxis));
			}
		}
		else
		{
			// calculate drag start position
			Math::plane axisplane = Math::plane(this->origin, zaxis);
			axisplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);
			this->dragStart = axispoint - Math::point(this->initialMatrix.get_position());

			axisplane = Math::plane(this->origin, lockedAxis);
			axisplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);
			this->lockedDragStart = axispoint - Math::point(this->initialMatrix.get_position());
		}
	}
}

void TranslateTool::Drag()
{
	BaseTool::Drag();
	
	this->deltaMatrix = Math::mat4::identity();

	// get mouse position on screen
	ImVec2 dockPos = ImGui::GetWindowPos();
	ImGuiStyle& style = ImGui::GetStyle();
	dockPos.x += style.WindowPadding.x;
	dockPos.y += style.WindowPadding.y;
	ImVec2 mousePos = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);
	ImVec2 dockSize = ImGui::GetWindowSize();

	// get mouse ray
	const float rayLength = 5000.0f;
	Math::line worldMouseRay = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mousePos.x, mousePos.y, rayLength, dockSize.x, dockSize.y);
	Math::vector translation;

	Math::plane viewPlane = this->viewPlane;
	viewPlane = Math::plane::normalize(viewPlane);

	Math::mat4 cameraTrans = Graphics::MainCamera::Instance()->getInvView();
	Math::vector zaxis = cameraTrans.get_zaxis();

	Math::mat4 rot = this->initialMatrix;
	rot.set_position(0);

	Math::mat4 localForward = Math::mat4::lookatlh(this->initialMatrix.get_position(), Math::point(this->initialMatrix.get_position() + rot.get_zaxis()), rot.get_yaxis());

	// update in X direction
	if (XAXIS == this->currentHandle)
	{
		Math::point axispoint;

		if (this->axisLockingMode)
		{
			zaxis.set_y(0);
			zaxis.set_z(0);

			Math::plane xplane = Math::plane(this->initialMatrix.get_position(), zaxis);
			xplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

			// the movement difference is then the one we perform in the view plane
			float diff1 = axispoint.y() - this->initialMatrix.get_position().y();
			float diff2 = axispoint.z() - this->initialMatrix.get_position().z();

			// also add the view-space offset from when we start dragging
			translation.set_y(diff1 - this->lockedDragStart.y());
			translation.set_z(diff2 - this->lockedDragStart.z());
		}
		else
		{
			// set x-factor of axis to 0
			zaxis.set_x(0);

			if (this->relativeMode)
			{
				// intersect with view plane
				viewPlane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

				// project our translation vector onto the coordinate vector relative to the object
				// also calculate projected startoff point for dragging
				Math::vec4 diff = axispoint - Math::point(this->initialMatrix.get_position());
				Math::vector relativeAxis = Math::mat4::transform(Math::vector(1, 0, 0), localForward);
				Math::vector trans = relativeAxis * (Math::vec4::dot3(diff, relativeAxis) / Math::vec4::dot3(relativeAxis, relativeAxis));

				translation.set_x(trans.x() - this->dragStart.x());
				translation.set_y(trans.y() - this->dragStart.y());
				translation.set_z(trans.z() - this->dragStart.z());
			}
			else
			{
				Math::plane xplane = Math::plane(this->initialMatrix.get_position(), zaxis);
				xplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

				// the movement difference is then the one we perform in the view plane
				float diff = axispoint.x() - this->initialMatrix.get_position().x();

				// also add the view-space offset from when we start dragging
				translation.set_x(diff - this->dragStart.x());
			}
		}
		if (this->snapMode)
		{
		#ifdef _DEBUG
			assert(this->snapOffset != 0);
		#endif
			translation.set_x(translation.x() - fmod(translation.x(), this->snapOffset) - fmod(initialMatrix.get_position().x(), snapOffset));
		}
	}
	// update in Y direction
	if (YAXIS == this->currentHandle)
	{
		Math::point axispoint;
		if (this->axisLockingMode)
		{
			zaxis.set_x(0);
			zaxis.set_z(0);

			Math::plane xplane = Math::plane(this->initialMatrix.get_position(), zaxis);
			xplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

			// the movement difference is then the one we perform in the view plane
			float diff1 = axispoint.x() - this->initialMatrix.get_position().x();
			float diff2 = axispoint.z() - this->initialMatrix.get_position().z();

			// also add the view-space offset from when we start dragging
			translation.set_x(diff1 - this->lockedDragStart.x());
			translation.set_z(diff2 - this->lockedDragStart.z());
		}
		else
		{
			// calculate intersection with arbitrary x/z-plane
			zaxis.set_y(0);

			if (this->relativeMode)
			{
				viewPlane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

				// project our translation vector onto the coordinate vector relative to the object
				// also calculate projected startoff point for dragging
				Math::vec4 diff = axispoint - Math::point(this->initialMatrix.get_position());
				Math::vector relativeAxis = Math::mat4::transform(Math::vector(0, 1, 0), localForward);
				Math::vector trans = relativeAxis * (Math::vec4::dot3(diff, relativeAxis) / Math::vec4::dot3(relativeAxis, relativeAxis));

				translation.set_x(trans.x() - this->dragStart.x());
				translation.set_y(trans.y() - this->dragStart.y());
				translation.set_z(trans.z() - this->dragStart.z());
			}
			else
			{
				Math::plane yplane = Math::plane(this->initialMatrix.get_position(), zaxis);
				yplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

				// the movement difference is then the one we perform in the view plane
				float diff = axispoint.y() - this->initialMatrix.get_position().y();

				// also add the view-space offset from when we start dragging
				translation.set_y(diff - this->dragStart.y());
			}
		}
		if (this->snapMode)
		{
		#ifdef _DEBUG
			assert(this->snapOffset != 0);
		#endif
			translation.set_y(translation.y() - fmod(translation.y(), this->snapOffset) - fmod(initialMatrix.get_position().y(), snapOffset));
		}
	}
	// update in Z direction
	if (ZAXIS == this->currentHandle)
	{
		Math::point axispoint;
		if (this->axisLockingMode)
		{
			zaxis.set_x(0);
			zaxis.set_y(0);

			Math::plane xplane = Math::plane(this->initialMatrix.get_position(), zaxis);
			xplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

			// the movement difference is then the one we perform in the view plane
			float diff1 = axispoint.x() - this->initialMatrix.get_position().x();
			float diff2 = axispoint.y() - this->initialMatrix.get_position().y();

			// also add the view-space offset from when we start dragging
			translation.set_x(diff1 - this->lockedDragStart.x());
			translation.set_y(diff2 - this->lockedDragStart.y());
		}
		else
		{
			// calculate intersection between axis line and mouse ray in the x/z-plane
			zaxis.set_z(0);

			if (this->relativeMode)
			{
				viewPlane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

				// project our translation vector onto the coordinate vector relative to the object
				// also calculate projected startoff point for dragging
				Math::vec4 diff = axispoint - Math::point(this->initialMatrix.get_position());
				Math::vector relativeAxis = Math::mat4::transform(Math::vector(0, 0, -1), localForward);
				Math::vector trans = relativeAxis * (Math::vec4::dot3(diff, relativeAxis) / Math::vec4::dot3(relativeAxis, relativeAxis));

				translation.set_x(trans.x() - this->dragStart.x());
				translation.set_y(trans.y() - this->dragStart.y());
				translation.set_z(trans.z() - this->dragStart.z());
			}
			else
			{
				Math::plane zplane = Math::plane(this->initialMatrix.get_position(), zaxis);
				zplane.intersectline(worldMouseRay.start(), worldMouseRay.end(), axispoint);

				// the movement difference is then the one we perform in the view plane
				float diff = axispoint.z() - this->initialMatrix.get_position().z();

				// also add the view-space offset from when we start dragging
				translation.set_z(diff - this->dragStart.z());
			}
		}
		if (this->snapMode)
		{
		#ifdef _DEBUG
			assert(this->snapOffset != 0);
		#endif
			translation.set_z(translation.z() - fmod(translation.z(), this->snapOffset) - fmod(initialMatrix.get_position().z(), snapOffset));
		}
	}
	// special mode, when origin is dragged
	if (ORIGIN == this->currentHandle)
	{
		ImVec2 dockSize = ImGui::GetWindowSize();

		// place matrix on a plane that goes through the last origin point of
		// the feature and is parallel to the camera clip planes
		Math::line centerViewLine = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(0.5f, 0.5f, 1, dockSize.x, dockSize.y);
		Math::vector viewVector = centerViewLine.vec();
		Math::vector v1 = FindOrtho(viewVector);
		Math::vector v2 = Math::vector::cross3(viewVector, v1);
		Math::plane translatePlane(this->startDragMatrix.get_position(), this->startDragMatrix.get_position() + v1, this->startDragMatrix.get_position() + v2);

		Math::vec4 t;
		translatePlane.intersectline(worldMouseRay.start(), worldMouseRay.end(), t);
		translation = t + this->dragStartMouseRayOffset - this->initialMatrix.get_position();

		if (this->snapMode)
		{
		#ifdef _DEBUG
			assert(this->snapOffset != 0);
		#endif
			translation.set(translation.x() - fmod(translation.x(), this->snapOffset) - fmod(initialMatrix.get_position().x(), snapOffset),
				translation.y() - fmod(translation.y(), this->snapOffset) - fmod(initialMatrix.get_position().y(), snapOffset),
				translation.z() - fmod(translation.z(), this->snapOffset) - fmod(initialMatrix.get_position().z(), snapOffset));
		}
	}

	this->deltaMatrix.translate(translation);
}

void TranslateTool::LeftUp()
{
	this->currentHandle = TransformHandle::NONE;
	BaseTool::LeftUp();
}

void TranslateTool::Render()
{
	const float lineWidth = 4.0f;

	// get mouse position on screen
	ImVec2 dockPos = ImGui::GetWindowPos();
	ImGuiStyle& style = ImGui::GetStyle();
	dockPos.x += style.WindowPadding.x;
	dockPos.y += style.WindowPadding.y;
	ImVec2 mousePos = ImVec2(ImGui::GetIO().MousePos.x - dockPos.x, ImGui::GetIO().MousePos.y - dockPos.y);

	ImVec2 dockSize = ImGui::GetWindowSize();

	// get mouse ray
	const float rayLength = 5000.0f;
	Math::line worldMouseRay = BaseGameFeature::EnvManager::Instance()->ComputeMouseWorldRay(mousePos.x, mousePos.y, rayLength, dockSize.x, dockSize.y);
	TransformHandle mode = NONE;
	if (!this->leftMouse)
	{
		mode = this->GetMouseHandle(worldMouseRay);
	}

	Math::vec4 color;
	Math::mat4 m;

	//-90 degrees
	Math::mat4 coneRotation = Math::mat4::rotationx(1.5707f);

	// get camera transform
	Math::mat4 cameraTrans = Graphics::MainCamera::Instance()->getInvView();

	// draw origin
	if (ORIGIN == this->currentHandle || ORIGIN == mode)
	{
		color.set(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		color.set(0.85f, 0.85f, 0.85f, 0.5f);
	}

	m = Math::mat4::identity();
	m.scale(Math::vector(this->handleScale, this->handleScale, this->handleScale));
	m.set_position(this->origin);
	Debug::DebugRenderer::Instance()->DrawBox(m, color, Debug::RenderMode::AlwaysOnTop);

	// draw X axis + handle
	if (XAXIS == this->currentHandle || XAXIS == mode)
	{
		if (this->axisLockingMode)	
			color.set(1, 1, 1, 1);
		else
			color.set(1, 1, 0, 1);
	}
	else
	{
		color.set(.8f, 0, 0, 1);
	}

	if (this->relativeMode)
	{
		Math::point axis = this->initialMatrix.get_xaxis();
		m = Math::mat4::lookatrh(Math::point(0, 0, 0), axis, this->initialMatrix.get_yaxis());
	}
	else
	{
		m = Math::mat4::lookatrh(Math::point(0, 0, 0), Math::point(1, 0, 0), Math::vector(0, 1, 0));
	}
	m = Math::mat4::multiply(coneRotation, m);
	m.scale(Math::vector(this->handleScale, this->handleScale, this->handleScale));
	m.set_position(this->xAxis);
	Debug::DebugRenderer::Instance()->DrawCone(m, color, Debug::RenderMode::AlwaysOnTop);

	Debug::DebugRenderer::Instance()->DrawLine(this->origin, this->xAxis, lineWidth, color, color, Debug::RenderMode::AlwaysOnTop);
	
	// draw Y axis + handle  
	if (YAXIS == this->currentHandle || YAXIS == mode)
	{
		if (this->axisLockingMode)
		{
			color.set(1, 1, 1, 1);
		}
		else
		{
			color.set(1, 1, 0, 1); 
		}
	}
	else
	{
		color.set(0, .8f, 0, 1);
	}
	if (this->relativeMode)
	{
		Math::point axis = this->initialMatrix.get_yaxis();
		m = Math::mat4::lookatrh(Math::point(0, 0, 0), axis, this->initialMatrix.get_zaxis());
	}
	else
	{
		m = Math::mat4::lookatrh(Math::point(0, 0, 0), Math::point(0, 1, 0), Math::vector(0, 0, 1));
	}
	coneRotation = Math::mat4::rotationx(1.5707f);
	m = Math::mat4::multiply(coneRotation, m);
	m.scale(Math::vector(this->handleScale, this->handleScale, this->handleScale));
	m.set_position(this->yAxis);
	Debug::DebugRenderer::Instance()->DrawCone(m, color, Debug::RenderMode::AlwaysOnTop);

	m = Math::mat4::identity();
	Debug::DebugRenderer::Instance()->DrawLine(this->origin, this->yAxis, lineWidth, color, color, Debug::RenderMode::AlwaysOnTop);

	// draw Z axis + handle    
	if (ZAXIS == this->currentHandle || ZAXIS == mode)
	{
		if (this->axisLockingMode)
		{
			color.set(1, 1, 1, 1);
		}
		else
		{
			color.set(1, 1, 0, 1);
		}
	}
	else
	{
		color.set(0, 0, .8f, 1);
	}

	if (this->relativeMode)
	{
		Math::point axis = this->initialMatrix.get_zaxis();
		m = Math::mat4::lookatrh(Math::point(0, 0, 0), axis, this->initialMatrix.get_yaxis());
	}
	else
	{
		m = Math::mat4::lookatrh(Math::point(0, 0, 0), Math::point(0, 0, 1), Math::vector(0, 1, 0));
	}
	coneRotation = Math::mat4::rotationx(-1.5707f);
	m = Math::mat4::multiply(coneRotation, m);
	m.scale(Math::vector(this->handleScale, this->handleScale, this->handleScale));
	m.set_position(this->zAxis);
	Debug::DebugRenderer::Instance()->DrawCone(m, color, Debug::RenderMode::AlwaysOnTop);

	m = Math::mat4::identity();
	Debug::DebugRenderer::Instance()->DrawLine(this->origin, this->zAxis, lineWidth, color, color, Debug::RenderMode::AlwaysOnTop);
}

/*
Computes the position of the handles and the scale of the whole feature.
The scale correction is used to display handles in a big size, even if they are
far away.
*/
void TranslateTool::UpdateHandlePositions()
{
	// compute origin position of feature
	this->origin = this->initialMatrix.get_position() + this->deltaMatrix.get_position();
	this->origin.set_w(1);

	// compute the scale factor
	Math::vector cameraPosition;
	float distanceToView;

	const Math::mat4 camTrans = Graphics::MainCamera::Instance()->getInvView();
	cameraPosition = camTrans.get_position();

	Math::vector v = (cameraPosition - this->origin);
	distanceToView = v.length();

	this->handleScale = distanceToView;

	// make the handles small
	this->handleScale *= 0.025f;

	Math::vector xdir, ydir, zdir;
	if (this->relativeMode)
	{
		Math::mat4 rot = this->initialMatrix;
		rot.set_position(Math::vec4(0));
		xdir = Math::vec4::normalize(Math::mat4::transform(Math::vector(1, 0, 0), rot)) * handleDistance * handleScale;
		ydir = Math::vec4::normalize(Math::mat4::transform(Math::vector(0, 1, 0), rot)) * handleDistance * handleScale;
		zdir = Math::vec4::normalize(Math::mat4::transform(Math::vector(0, 0, 1), rot)) * handleDistance * handleScale;
	}
	else
	{
		xdir = Math::vector(this->handleDistance * this->handleScale, 0.0f, 0.0f);
		ydir = Math::vector(0.0f, this->handleDistance * this->handleScale, 0.0f);
		zdir = Math::vector(0.0f, 0.0f, this->handleDistance * this->handleScale);
	}

	// scale handles
	this->xAxis = this->origin + xdir;
	this->yAxis = this->origin + ydir;
	this->zAxis = this->origin + zdir;

	// create view plane
	Math::vector forward(0, 0, 1);
	forward = Math::mat4::transform(forward, camTrans);
	this->viewPlane = Math::plane(this->origin, forward);
}

void TranslateTool::UpdateTransform(const Math::mat4& transform)
{
	this->initialMatrix = transform;
	this->deltaMatrix = Math::mat4::identity();
}

}