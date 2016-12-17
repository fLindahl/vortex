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

void SurfaceCollider::CookMeshData(const std::shared_ptr<Render::MeshResource> mesh)
{
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

void SurfaceCollider::CookOBJData(Util::Array<Render::MeshResource::OBJVertex>& mesh, Util::Array<unsigned int>& indices, const Math::bbox& bbox)
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

	this->colliderbbox = bbox;
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