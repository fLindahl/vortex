#pragma once
#include <stddef.h>

#pragma pack(push, 1)

namespace Render
{

struct Nvx2Header
{
	uint magic;
	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	uint numIndices;
	uint numEdges;
	uint vertexComponentMask;
};

struct Nvx2Group
{
	uint firstVertex;
	uint numVertices;
	uint firstTriangle;
	uint numTriangles;
	uint firstEdge;
	uint numEdges;
};

}

#pragma pack(pop)