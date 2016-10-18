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
#include "vertexcomponent.h"

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

		Math::Vector4 pos;
		Math::Vector3 normal;
		Math::Vector2 uv;
	};

	bool loadMeshFromFile(const char* filename);
	bool loadMeshFromOBJ(const char* filename);

	void createQuad();
	///Bind VAO
	void Bind();
	/// Unbind VAO
	void Unbind();
	///Draw elements
	void Draw();

	void* getMesh();
	void* getIndices();

private:
	GLuint vao[1];
	GLuint ib[1];
	GLuint vbo[1];

	void* mesh;
	void* indices;

	//Util::Array < MeshResource::Vertex > mesh;
	//Util::Array < GLuint > indices;

	bool rawMode;

	void ReadHeaderData();
	void ReadPrimitiveGroups();
	void SetupVertices();

	void SetupVertexBuffer();
	void SetupIndexBuffer();

	uchar* mapPtr;
	void* groupDataPtr;
	void* vertexDataPtr;
	void* indexDataPtr;

	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	uint numIndices;
	uint numEdges;
	uint vertexComponentMask;

	size_t groupDataSize;
	size_t vertexDataSize;
	size_t indexDataSize;

	Util::Array<VertexComponent> vertexComponents;

};

}