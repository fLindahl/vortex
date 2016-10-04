#pragma once
#include <cstddef>
#include "GL/glew.h"
#include <string>
#include <fstream>
#include "foundation/math/vector4.h"
#include "foundation/math/vector3.h"
#include "foundation/math/vector2.h"
#include <map>
#include <utility>
#include "foundation/util/array.h"

namespace Render
{

class MeshResource
{
public:
	MeshResource();
	~MeshResource();

	struct Vertex
	{
		bool operator==(const Vertex& rhs) const
		{
			return (pos == rhs.pos && uv == rhs.uv && normal == rhs.normal);
		}

		Math::Vector3 pos;
		Math::Vector2 uv;
		Math::Vector3 normal;
	};

	bool loadMeshFromFile(const char* filename);

	void setupMesh();

	void createQuad();
	void draw();
	Util::Array < MeshResource::Vertex >& getMesh();
	Util::Array < GLuint >& getIndices();

private:
	GLuint vao[1];
	GLuint ib[1];
	GLuint vbo[1];

	Util::Array < MeshResource::Vertex > mesh;
	Util::Array < GLuint > indices;
};

}