#pragma once
#include "foundation/math/plane.h"
#include "basecollider.h"
#include "foundation/math/point.h"
#include "render/resources/meshresource.h"

namespace Physics
{
	

class SurfaceCollider : public BaseCollider
{	
public:
	struct ColliderFace
	{
		//TODO: Can we use pointers here? Are we sure that the meshdata is 16-byte aligned?
		Math::point p0;
		Math::point p1;
		Math::point p2;
		//Math::plane plane;
	};

	SurfaceCollider();
	~SurfaceCollider();

	/// Cook a physics surface collider from mesh data.
	/// meshdata: A pointer to the first element in the meshdata that you want to cook.
	/// indexdata: A pointer to the first element in the indexdata array.
	/// vertexstride: How large (in bytes) is our entire vertex-structure (how far between position elements)?
	/// indexstride: How large (in bytes) are our indices?
	/// positionoffset: Is there any offset before we hit our positional data in the vertex-structure?
	void CookMeshData(void* meshdata, void* indexdata, index_t numindices, size_t vertexstride, size_t indexstride, size_t positionoffset);
	
	//TEMPORARY: for cooking an OBJ-Loaded Mesh
	void CookOBJData(Util::Array<Render::MeshResource::OBJVertex>& mesh, Util::Array<unsigned int>& indices, const Math::bbox& bbox);

	Util::Array<ColliderFace>& GetFaceList() { return this->faces; }

	void debugDraw();

private:

	Util::Array<ColliderFace> faces;
};

}