#include "config.h"
#include "convexhullcollider.h"
#include "physicsserver.h"

namespace Physics
{

__ImplementClass(Physics::ConvexHullCollider, 'CHCL', Physics::BaseCollider);

ConvexHullCollider::ConvexHullCollider()
{
	this->btCollider = new btConvexHullShape();
}

ConvexHullCollider::~ConvexHullCollider()
{
	if (this->btCollider != nullptr)
		delete this->btCollider;
}

void ConvexHullCollider::CookMeshData(const Ptr<Render::MeshResource> mesh)
{

	this->baseMesh = mesh;

	btConvexHullShape* shape = new btConvexHullShape((float*)mesh->getMesh(), mesh->getNumVertices(), mesh->getVertexWidth());
	
	//create a hull approximation to simplify the mesh allowing for coarser but faster calculations
	delete this->hull;
	this->hull = new btShapeHull(shape);
	btScalar margin = shape->getMargin();
	hull->buildHull(margin);
	
	*this->btCollider = btConvexHullShape((float*)hull->getVertexPointer(), hull->numVertices());

	delete shape;
}

const Ptr<Render::MeshResource>& ConvexHullCollider::GetMesh() const
{
	return this->baseMesh;
}

}