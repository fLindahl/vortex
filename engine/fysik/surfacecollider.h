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
	void CookMeshData(const std::shared_ptr<Render::MeshResource> mesh);

	Util::Array<ColliderFace>& GetFaceList() { return this->faces; }

	void debugDraw();

private:

	Util::Array<ColliderFace> faces;
};

}