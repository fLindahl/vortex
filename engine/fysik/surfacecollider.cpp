#include "config.h"
#include "surfacecollider.h"

namespace Physics
{

void SurfaceCollider::CookMeshData(void* meshdata, void* indexdata, index_t numindices, size_t vertexstride, size_t indexstride, size_t positionoffset)
{
	printf("NOT IMPLEMENTED");
	assert(false);

	Math::point* pointer = (Math::point*)meshdata + positionoffset;

	uint* index = (uint*)indexdata;

	ColliderFace face;

	for (index_t i = 0; i < numindices; i++)
	{
		

		index = index + indexstride;
	}

}

void SurfaceCollider::CookOBJData(Util::Array<Render::MeshResource::OBJVertex>& mesh, Util::Array<index_t>& indices)
{
	for (size_t i = 0; i < indices.Size(); i += 3)
	{
		ColliderFace face;

		face.p0 = Math::point(mesh[indices[i]].pos[0], mesh[indices[i]].pos[1], mesh[indices[i]].pos[2]);
		face.p1 = Math::point(mesh[indices[i + 1]].pos[0], mesh[indices[i + 1]].pos[1], mesh[indices[i + 1]].pos[2]);
		face.p2 = Math::point(mesh[indices[i + 2]].pos[0], mesh[indices[i + 2]].pos[1], mesh[indices[i + 2]].pos[2]);
		//face.plane = Math::plane(face.p0, face.p1, face.p2);

		this->faces.Append(face);
	}
}


}