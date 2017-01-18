#pragma once
#include "config.h"

#pragma pack(push, 1)

namespace Render
{

struct MeshFileHeader
{
	uint magic;
	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	uint numIndices;
	uint vertexComponentMask;
};

struct MeshFilePrimitiveGroup
{
	//256 is primitivegroup name limit!
	char name[256];
	size_t indexpointer;
	size_t numindices;
	//uint firstVertex;
	//uint numVertices;
	//uint firstTriangle;
	//uint numTriangles;
	//uint firstEdge;
	//uint numEdges;
};

}

#pragma pack(pop)