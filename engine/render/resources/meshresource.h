#pragma once
#include <cstddef>
#include "GL/glew.h"
#include <string>
#include <fstream>
#include "foundation/math/vector4.h"
#include <map>
#include <utility>
#include <foundation/math/bbox.h>
#include "foundation/util/array.h"
#include "vertexcomponent.h"

namespace Render
{

	struct PrimitiveGroup
	{
		//name of this primitivegroup
		std::string name;

		///Surfaceindex for this primitivegroup.
		///Warning: Do not use this unless you know what you're doing! (read: Consulting Fredrik)
		unsigned int surfaceIndex;

		//Pointer to this primitivegroups first element in the total buffer
		size_t indexOffset;

		//amount of floats that fit a vertex
		//uint vertexWidth;
		//Number of indices in this group
		size_t numIndices;

		//How many bytes are our index buffer?
		size_t indexDataSize;

		//Util::Array<VertexComponent> vertexComponents;
	};

class MeshResource
{
public:
	MeshResource();
	~MeshResource();

	bool loadMesh(const char* filename);

	bool loadMeshFromFile(const char* filename);

	///Bind VAO
	void Bind();
	/// Unbind VAO
	void Unbind();
	///Draw elements
	void Draw(const unsigned int& primitiveGroup = 0);

	void* getMesh();
	void* getIndices();

	const PrimitiveGroup& getPrimitiveGroup(const int& i) { return this->primitiveGroups[i]; }

	///Total amount of indices in mesh
	size_t getNumIndices() { return this->numIndices; }
	///Total amount of vertices in mesh
	uint getNumVertices() { return this->numVertices; }
	///Returns the amount of floats that a vertex contains
	uint getVertexWidth() { return this->vertexWidth; }

	size_t getNumPrimitiveGroups() { return this->primitiveGroups.Size(); }

	///Returns the base bbox for this entity.
	Math::bbox getBaseBBox() { return this->bbox; }

	std::string GetName() { return this->name; }
	std::string GetPath() { return this->filePath; }

private:
	// The filename for this mesh
	std::string name;
	std::string filePath;

	Math::bbox bbox;
	///Contains all PrimitiveGroups vertices/indices
	void* mesh;
	void* indices;

	///PrimitiveGroups for this mesh
	Util::Array<PrimitiveGroup> primitiveGroups;
	
	GLuint vao[1];
	GLuint ib[1];
	GLuint vbo[1];

	bool rawMode;
	bool N2File = false;

	uchar* mapPtr;
	void* groupDataPtr;
	void* vertexDataPtr;
	void* indexDataPtr;

	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	size_t numIndices;
	uint numEdges;
	uint vertexComponentMask;

	size_t groupDataSize;
	size_t vertexDataSize;
	size_t indexDataSize;

	//Functions

	///Setup GL Buffers
	void SetupVertexBuffer();
	void SetupIndexBuffer();

	///Read nvx2 data
	void ReadHeaderData();
	void ReadPrimitiveGroups();
	void SetupVertices();

	Util::Array<VertexComponent> vertexComponents;
};

}