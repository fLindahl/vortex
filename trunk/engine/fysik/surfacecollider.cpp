#include "config.h"
#include "surfacecollider.h"

namespace Physics
{

SurfaceCollider::SurfaceCollider()
{

}

SurfaceCollider::~SurfaceCollider()
{

}

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

void SurfaceCollider::CookOBJData(Util::Array<Render::MeshResource::OBJVertex>& mesh, Util::Array<unsigned int>& indices)
{
	size_t iSize = indices.Size();
	for (size_t i = 0; i < iSize; i += 3)
	{
		ColliderFace face;

		face.p0 = Math::point(mesh[indices[i]].pos[0], mesh[indices[i]].pos[1], mesh[indices[i]].pos[2]);
		face.p1 = Math::point(mesh[indices[i + 1]].pos[0], mesh[indices[i + 1]].pos[1], mesh[indices[i + 1]].pos[2]);
		face.p2 = Math::point(mesh[indices[i + 2]].pos[0], mesh[indices[i + 2]].pos[1], mesh[indices[i + 2]].pos[2]);
		//face.plane = Math::plane(face.p0, face.p1, face.p2);

		this->faces.Append(face);
	}
}


void SurfaceCollider::debugDraw()
{
	glUseProgram(0);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&Graphics::MainCamera::Instance()->getViewMatrix().mat.m[0][0]);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)&Graphics::MainCamera::Instance()->getProjectionMatrix().mat.m[0][0]);

	glBegin(GL_POINTS);

	glColor3f(0.5f, 0.5f, 0.5f);

	for(auto face : faces)
	{
		glVertex4f(face.p0[0], face.p0[1], face.p0[2], face.p0[3]);
		glVertex4f(face.p1[0], face.p1[1], face.p1[2], face.p1[3]);
		glVertex4f(face.p2[0], face.p2[1], face.p2[2], face.p2[3]);
	}

	glEnd();
}

}