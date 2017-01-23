#include "rotatetool.h"
#include "foundation/math/math.h"

namespace Tools
{

RotateTool::RotateTool() :
	handleDistance(5.0f),
	outerCircleScale(1.2f),
	startDragOrientation(0.0f, 0.0f, 0.0f)
{
	this->startDragMatrix = Math::mat4::identity();
	this->lastStartDragDeltaMatrix = Math::mat4::identity();
	type = ToolType::ROTATE;
}
	
RotateTool::~RotateTool()
{
}

TransformHandle RotateTool::GetMouseHandle(const Math::line& worldMouseRay)
{
	const float maxAngleDifference(0.2f);
	const float outerRadiusTolerance(0.5f);

	// define picking sphere radius
	float radius(this->handleDistance * this->handleScale);
	float outerRadius(radius * this->outerCircleScale);

	// get collision Math::point of ray with sphere
	Math::vector collisionPoint = GetSphereIntersectionPoint(worldMouseRay, this->origin, radius);

	// Math::vector from collision Math::point to handle origin
	Math::vector colVec = collisionPoint - this->origin;
	colVec = Math::vector::normalize(colVec);

	if (worldMouseRay.distance(this->origin) < radius)
	{
		float closestHandle = 0;
		Math::vector plane_normal1, plane_normal2, plane_normal3;
		TransformHandle retval = NONE;
		float smallestAngle = FLT_MAX;
		TransformHandle modes[] = { XAXIS, YAXIS, ZAXIS };
		Math::vector normals[] = { xPlane.n(), yPlane.n(), zPlane.n() };

		index_t i;
		for (i = 0; i < 3; i++)
		{
			Math::vector normal = normals[i];
			float angle;
			normal = Math::vector::normalize(normal);
			angle = 0.5f*(float)PI - ((float)acos(Math::vector::dot3(normal, colVec)));
			if (abs(angle) < maxAngleDifference &&
				abs(angle) < smallestAngle)
			{
				retval = modes[i];
				smallestAngle = angle;
			}
		}

		return retval;
	}
	else
	{
		float distanceRayHandle(worldMouseRay.distance(this->origin));
		if (distanceRayHandle < outerRadius + outerRadiusTolerance * this->handleScale &&
			distanceRayHandle > outerRadius - outerRadiusTolerance * this->handleScale)
		{
			return VIEWAXIS;
		}
	}

	return NONE;
}

//------------------------------------------------------------------------------
/**
Checks mouse position and if it's over a rotation feature handle
start the rotation mode.
*/
void RotateTool::LeftDown()
{
	BaseTool::LeftDown();
	/*
	// store copy of delta matrix
	this->lastStartDragDeltaMatrix = this->deltaMatrix;

	this->xDragStart = this->xAxis;
	this->yDragStart = this->yAxis;
	this->zDragStart = this->zAxis;

	// get mouse position on screen
	const float2& mousePos = Input::InputServer::Instance()->GetDefaultMouse()->GetScreenPosition();

	// get mouse ray
	const float rayLength = 5000.0f;
	Math::line worldMouseRay = envQueryManager->ComputeMouseWorldRay(mousePos, rayLength, defaultView);

	// define picking sphere radius
	float radius(this->handleDistance * this->handleScale);
	float distanceRayHandle(worldMouseRay.distance(this->origin));

	// get collision Math::point of ray with sphere
	Math::vector collisionPoint = GetSphereIntersectionPoint(worldMouseRay, this->origin, radius);

	// Math::vector from collision Math::point to handle origin
	Math::vector colVec = collisionPoint - this->origin;
	colVec = Math::vector::normalize(colVec);

	// calculate start drag orientation
	this->startDragOrientation = this->ComputeDragVector(worldMouseRay, this->currentHandle);
	*/
}

//------------------------------------------------------------------------------
/**
Checks mouse position, and if in active drag mode, it will update the
delta rotation matrix.
*/
void RotateTool::Drag()
{
	BaseTool::Drag();
	/*
	const float2& mousePos = Input::InputServer::Instance()->GetDefaultMouse()->GetScreenPosition();

	// get mouse ray
	const float rayLength = 5000.0f;
	Math::line worldMouseRay = envQueryManager->ComputeMouseWorldRay(mousePos, rayLength, defaultView);
	Math::vector currentDragVec = this->ComputeDragVector(worldMouseRay, this->currentHandle);


	//float angleDiff = n_acos(Math::vector::dot3(currentDragVec, this->startDragOrientation));
	//Math::vector cross = Math::vector::cross3(currentDragVec, this->startDragOrientation);
	//if (Math::vector::dot3(this->view_plane.n(), cross) >= 0) angleDiff = -angleDiff;

	if (XAXIS == this->currentHandle)
	{
		float dot = Math::vector::dot3(currentDragVec, this->startDragOrientation);
		float det = Math::vector::dot3(this->xAxis, Math::vector::cross3(this->startDragOrientation, currentDragVec));
		float angleDiff = atan2f(det, dot);

		this->deltaMatrix = Math::mat4::rotationaxis(this->xAxis, angleDiff);
	}
	else if (YAXIS == this->currentHandle)
	{
		float dot = Math::vector::dot3(currentDragVec, this->startDragOrientation);
		float det = Math::vector::dot3(this->yAxis, Math::vector::cross3(this->startDragOrientation, currentDragVec));
		float angleDiff = atan2f(det, dot);

		this->deltaMatrix = Math::mat4::rotationaxis(this->yAxis, angleDiff);
	}
	else if (ZAXIS == this->currentHandle)
	{
		float dot = Math::vector::dot3(currentDragVec, this->startDragOrientation);
		float det = Math::vector::dot3(this->zAxis, Math::vector::cross3(this->startDragOrientation, currentDragVec));
		float angleDiff = atan2f(det, dot);

		this->deltaMatrix = Math::mat4::rotationaxis(this->zAxis, angleDiff);
	}
	else if (VIEWAXIS == this->currentHandle)
	{
		float dot = Math::vector::dot3(currentDragVec, this->startDragOrientation);
		float det = Math::vector::dot3(this->viewAxis, Math::vector::cross3(this->startDragOrientation, currentDragVec));
		float angleDiff = atan2f(det, dot);

		this->deltaMatrix = Math::mat4::rotationaxis(this->viewAxis, angleDiff);
	}
	*/
}

//------------------------------------------------------------------------------
/**
Computes a Math::vector that points to the intersection Math::point of given Math::line with
a layer depending of the given drag mode. The angle between vectors computed
by this method can used to calculate the rotation delta of the feature.
*/
Math::vector
RotateTool::ComputeDragVector(const Math::line& ray, TransformHandle mode)
{
	Math::vec4 t;
	this->viewPlane.intersectline(ray.start(), ray.end(), t);
	return Math::vector::normalize(t - this->origin);
}

//------------------------------------------------------------------------------
/**
This is called when the user finished dragging a feature
*/
void
RotateTool::LeftUp()
{
	BaseTool::LeftUp();
}

//------------------------------------------------------------------------------
/**
Renders the rotation handles, which can be dragged
*/
void
RotateTool::Render()
{
	/*
	// The following part is needed to get a mouse over visualization
	// for the handles. For thta calculations like in StartDrag() are
	// required.

	// define picking sphere radius
	float radius(this->handleDistance * this->handleScale);

	// get mouse position on screen
	const float2& mousePos = Input::InputServer::Instance()->GetDefaultMouse()->GetScreenPosition();

	// get mouse ray
	const float rayLength = 5000.0f;
	Math::line worldMouseRay = envQueryManager->ComputeMouseWorldRay(mousePos, rayLength, defaultView);
	TransformHandle mode = NONE;
	if (!this->isInDragMode) mode = this->GetMouseHandle(worldMouseRay);

	// get collision Math::point of ray with sphere
	Math::point collisionPoint = GetSphereIntersectionPoint(worldMouseRay, this->origin, radius);

	// compute angle between Math::vector from collision Math::point to handle origin
	// and axis planes.
	Math::vector colVec = collisionPoint - this->origin;
	colVec = Math::vector::normalize(colVec);

	// declare some variables
	Math::vec4 color;
	const int lineCount = 40;
	Math::vector help;
	CoreGraphics::RenderShape::RenderShapeVertex handlePoints[lineCount * 2];
	Math::vector circleVector;
	Math::vector planeNormal;
	index_t i;
	Math::line intersectionLine;
	planeNormal = Math::vector(this->viewPlane.a(), this->viewPlane.b(), this->viewPlane.c());

	// compute and draw x handle points    
	planeNormal = Math::vector(this->xPlane.a(), this->xPlane.b(), this->xPlane.c());
	PlaneIntersect(this->viewPlane, this->xPlane, intersectionLine);
	circleVector = intersectionLine.vec();
	circleVector = Math::vector::normalize(circleVector);
	circleVector *= this->handleDistance * this->handleScale;

	if (XAXIS == this->currentHandle || XAXIS == mode)
	{
		color = Math::vec4(1, 1, 0, 1);
	}
	else
	{
		color = Math::vec4(.8f, 0, 0, 1);
	}

	for (i = 0; i < (lineCount * 2) - 1; i += 2)
	{
		help.set(circleVector.x(), circleVector.y(), circleVector.z());
		this->RotateVector(help, this->xAxis, (float)-PI * (i / 2) / lineCount);
		handlePoints[i].pos = this->origin + help;

		help = circleVector;
		this->RotateVector(help, this->xAxis, (float)-PI * (i / 2 + 1) / lineCount);
		handlePoints[i + 1].pos = this->origin + help;

		handlePoints[i].color = color;
		handlePoints[i + 1].color = color;
	}

	// draw X axis
	Debug::DebugShapeRenderer::Instance()->DrawPrimitives(Math::mat4::identity(),
		CoreGraphics::PrimitiveTopology::LineList,
		lineCount,
		handlePoints,
		color,
		CoreGraphics::RenderShape::AlwaysOnTop);

	// compute and draw y handle points    
	PlaneIntersect(this->viewPlane, this->yPlane, intersectionLine);
	circleVector = intersectionLine.vec();
	circleVector = Math::vector::normalize(circleVector);
	circleVector *= this->handleDistance * this->handleScale;


	if (YAXIS == this->currentHandle || YAXIS == mode)
	{
		color = Math::vec4(1, 1, 0, 1);
	}
	else
	{
		color = Math::vec4(0, .8f, 0, 1);
	}

	for (i = 0; i<(lineCount * 2) - 1; i += 2)
	{
		help = circleVector;
		this->RotateVector(help, this->yAxis, (float)PI * (i / 2) / lineCount);
		handlePoints[i].pos = this->origin + help;

		help = circleVector;
		this->RotateVector(help, this->yAxis, (float)PI * (i / 2 + 1) / lineCount);
		handlePoints[i + 1].pos = this->origin + help;

		handlePoints[i].color = color;
		handlePoints[i + 1].color = color;
	}

	// draw Y axis
	Debug::DebugShapeRenderer::Instance()->DrawPrimitives(Math::mat4::identity(),
		CoreGraphics::PrimitiveTopology::LineList,
		lineCount,
		handlePoints,
		color,
		CoreGraphics::RenderShape::AlwaysOnTop);

	// compute and draw z handle points    
	PlaneIntersect(this->viewPlane, this->zPlane, intersectionLine);
	circleVector = intersectionLine.vec();
	circleVector = Math::vector::normalize(circleVector);
	circleVector *= this->handleDistance * this->handleScale;

	if (ZAXIS == this->currentHandle || ZAXIS == mode)
	{
		color = Math::vec4(1, 1, 0, 1);
	}
	else
	{
		color = Math::vec4(0, 0, .8f, 1);
	}

	for (i = 0; i<(lineCount * 2) - 1; i += 2)
	{
		help = circleVector;
		this->RotateVector(help, this->zAxis, -(float)PI * (i / 2) / lineCount);
		handlePoints[i].pos = this->origin + help;

		help = circleVector;
		this->RotateVector(help, this->zAxis, -(float)PI * (i / 2 + 1) / lineCount);
		handlePoints[i + 1].pos = this->origin + help;

		handlePoints[i].color = color;
		handlePoints[i + 1].color = color;
	}

	// draw Z axis
	Debug::DebugShapeRenderer::Instance()->DrawPrimitives(Math::mat4::identity(),
		CoreGraphics::PrimitiveTopology::LineList,
		lineCount,
		handlePoints,
		color,
		CoreGraphics::RenderShape::AlwaysOnTop);

	// draw outer circle
	Math::vector normal = Math::vector(this->viewPlane.a(), this->viewPlane.b(), this->viewPlane.c());
	circleVector = FindOrtho(normal);
	circleVector = Math::vector::normalize(circleVector);
	circleVector *= this->handleDistance * this->handleScale * this->outerCircleScale;

	if (VIEWAXIS == this->currentHandle || VIEWAXIS == mode)
	{
		color = Math::vec4(1, 1, 0, 1);
	}
	else
	{
		color = Math::vec4(.5f, .5f, 1.0f, 1.0f);
	}

	for (i = 0; i<(lineCount * 2) - 1; i += 2)
	{
		help = circleVector;
		this->RotateVector(help, normal, 2 * (float)PI * (i / 2) / lineCount);
		handlePoints[i].pos = this->origin + help;

		help = circleVector;
		this->RotateVector(help, normal, 2 * (float)PI * (i / 2 + 1) / lineCount);
		handlePoints[i + 1].pos = this->origin + help;

		handlePoints[i].color = color;
		handlePoints[i + 1].color = color;
	}

	// draw it
	Debug::DebugShapeRenderer::Instance()->DrawPrimitives(Math::mat4::identity(),
		CoreGraphics::PrimitiveTopology::LineList,
		lineCount,
		handlePoints,
		color,
		CoreGraphics::RenderShape::AlwaysOnTop);

	*/
}

// ------------------------------------------------------------------------------
/**
Computes different variables, that are used for picking and rendering handles.
*/
void
RotateTool::UpdateHandlePositions()
{
	/*
	this->DecomposeInitialMatrix();

	Ptr<BaseGameFeature::EnvQueryManager> envQueryManager = BaseGameFeature::EnvQueryManager::Instance();

	// compute the scale factor
	Math::vector cameraPosition;
	float distanceToView;

	n_assert(cameraEntity.isvalid());
	const Math::mat4 camTrans = cameraEntity->GetMatrix44(Attr::Transform);
	cameraPosition = camTrans.get_position();

	// calculate scale of handles relative to the distance to the camera
	Math::vector v = (cameraPosition - this->origin);
	distanceToView = v.length();
	this->handleScale = distanceToView;

	// make the handles small
	this->handleScale *= 0.03f;

	this->xAxis = Math::mat4::transform(Math::vector(1, 0, 0), this->deltaMatrix);
	this->yAxis = Math::mat4::transform(Math::vector(0, 1, 0), this->deltaMatrix);
	this->zAxis = Math::mat4::transform(Math::vector(0, 0, 1), this->deltaMatrix);
	this->origin = (this->decomposedTranslation);

	// create orthogonal planes to the feature axis
	this->xPlane.setup_from_points(this->origin, this->origin + Math::point(this->yAxis), this->origin + Math::point(this->zAxis));
	this->yPlane.setup_from_points(this->origin, this->origin + Math::point(this->xAxis), this->origin + Math::point(this->zAxis));
	this->zPlane.setup_from_points(this->origin, this->origin + Math::point(this->xAxis), this->origin + Math::point(this->yAxis));

	// calculate view Math::plane
	Math::vector forward(0, 0, -1);
	forward = Math::mat4::transform(forward, camTrans);
	this->viewPlane.setup_from_point_and_normal(this->origin, forward);
	this->viewAxis = this->viewPlane.n();
	*/
}

// ------------------------------------------------------------------------------
/**
Gets the closest Math::point of intersection of Math::line and sphere.
If no intersection occurred, it returns the closest Math::point on sphere to Math::line.
*/
Math::vector
RotateTool::GetSphereIntersectionPoint(const Math::line& ray, const Math::vector& location, float radius)
{
	Math::vector collisionPoint;
	/*

	float distanceRayLoc(ray.distance(location));

	// get shortest distance Math::point on ray Math::line
	float tForClosestPoint(ray.closestpoint(location));
	Math::vector closestPoint = ray.pointat(tForClosestPoint);
	//Math::vector closestPoint(ray.ipol(tForClosestPoint));

	if (distanceRayLoc<radius)
	{
		// get distance of that Math::point to sphere intersection points
		float distance(sqrt(radius * radius - distanceRayLoc * distanceRayLoc));

		// compute intersection Math::point of ray with sphere
		// get closes Math::point to ray start by "walking back" in the direction of the ray Math::vector
		// starting at from closest Math::point on Math::line to handle origin
		Math::vector rayNormal(ray.vec());
		rayNormal = Math::vector::normalize(rayNormal);// rayNormal.norm();
		collisionPoint = (closestPoint + (rayNormal * (-distance)));
	}
	else
	{
		// calculate closest Math::point on sphere
		collisionPoint = (location + (closestPoint - location) * radius);
	}

	*/
	return collisionPoint;
}

//------------------------------------------------------------------------------
/**
*/
void
RotateTool::RotateVector(Math::vector& i_v, Math::vector& axis, float angle)
{
	Math::mat4 rot = Math::mat4::rotationaxis(axis, angle);
	i_v = Math::mat4::transform(i_v, rot);
}

//------------------------------------------------------------------------------
/**
*/
bool
RotateTool::PlaneIntersect(Math::plane& p1, Math::plane &p2, Math::line& l)
{
	/*
	Math::vector n0(p1.a(), p1.b(), p1.c());
	Math::vector n1(p2.a(), p2.b(), p2.c());
	float n00 = Math::vector::dot3(n0, n0);
	float n01 = Math::vector::dot3(n0, n1);
	float n11 = Math::vector::dot3(n1, n1);
	float det = n00 * n11 - n01 * n01;
	const float tol = 1e-06f;
	if (fabs(det) < tol)
	{
		return false;
	}
	else
	{
		float inv_det = 1.0f / det;
		float c0 = (n11 * p1.d() - n01 * p2.d())* inv_det;
		float c1 = (n00 * p2.d() - n01 * p1.d())* inv_det;
		l.m = Math::vector::cross3(n0, n1);
		l.b = n0 * c0 + n1 * c1;
		return true;
	}
	*/
	//remove!
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void RotateTool::GetAnglesFromMat(const Math::mat4& i_m, float& x, float& y, float& z)
{
	/*
	//extract rotation from matrix
	quaternion _q = Math::mat4::rotationmatrix(i_m);
	Math::mat4 m = Math::mat4::rotationquaternion(_q);

	z = atan2(m.getrow1().x(), m.getrow0().x());
	y = atan2(-m.getrow2().x(), sqrt(pow(m.getrow2().y(), 2) + pow(m.getrow2().z(), 2)));
	x = atan2(m.getrow2().y(), m.getrow2().z());
	*/
}

}