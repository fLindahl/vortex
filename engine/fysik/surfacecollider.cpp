#include "config.h"
#include "surfacecollider.h"

namespace Physics
{

__ImplementClass(Physics::SurfaceCollider, 'SCOL', Physics::BaseCollider);

SurfaceCollider::SurfaceCollider()
{

}

SurfaceCollider::~SurfaceCollider()
{

}

void SurfaceCollider::CookMeshData(const Ptr<Render::MeshResource> mesh)
{
	this->faces.Clear();

	float* vertDataBase = (float*)mesh->getMesh();
	const uint vWidth = mesh->getVertexWidth();

	uint* index = (uint*)mesh->getIndices();

	ColliderFace face;

	for (index_t i = 0; i < mesh->getNumIndices(); i += 3)
	{
		float* data = vertDataBase + (*index) * vWidth;
		face.p0 = Math::point(data[0], data[1], data[2]);
		data = vertDataBase + (*(index + 1)) * vWidth;
		face.p1 = Math::point(data[0], data[1], data[2]);
		data = vertDataBase + (*(index + 2)) * vWidth;
		face.p2 = Math::point(data[0], data[1], data[2]);

		this->faces.Append(face);
		index = index + 3;
	}

	this->colliderbbox = mesh->getBaseBBox();
}

Util::Array<ColliderFace>& SurfaceCollider::GetFaceList()
{
	return this->faces;
}

const Ptr<Render::MeshResource>& SurfaceCollider::GetMesh() const
{
	return this->baseMesh;
}

}