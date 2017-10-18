#pragma once
#include "core/refcounted.h"
#include "foundation/math/plane.h"
#include "basecollider.h"
#include "foundation/math/point.h"
#include "render/resources/meshresource.h"
#include "BulletCollision\CollisionShapes\btShapeHull.h"

namespace Physics
{

class ConvexHullCollider : public BaseCollider
{	
	__DeclareClass(ConvexHullCollider)
public:
	ConvexHullCollider();
	~ConvexHullCollider();

	/// Cook a convex hull collider from mesh data.
	void CookMeshData(const Ptr<Render::MeshResource> mesh);

	const Ptr<Render::MeshResource>& GetMesh() const;

private:
	Ptr<Render::MeshResource> baseMesh;

	///Used to store the hull since it's a simplified mesh;
	btShapeHull* hull;
};

}