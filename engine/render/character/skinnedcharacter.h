#pragma once
#include <cstddef>
#include "GL/glew.h"
#include <string>
#include <fstream>
#include <vector>
#include "foundation/math/vector4.h"
#include <map>
#include <utility>
#include "foundation/util/array.h"
#include <config.h>
#include "characterskeleton.h"
#include "render/resources/vertexcomponent.h"

namespace Render
{

class SkinnedCharacter{
public:
	SkinnedCharacter();
	~SkinnedCharacter();

	enum N2VertexComponent
	{
		N2Coord = (1 << 0),      // 3 floats
		N2Normal = (1 << 1),      // 3 floats
		N2NormalB4N = (1 << 2),      // 4 unsigned bytes, normalized
		N2Uv0 = (1 << 3),      // 2 floats
		N2Uv0S2 = (1 << 4),      // 2 shorts, 4.12 fixed point
		N2Uv1 = (1 << 5),      // 2 floats
		N2Uv1S2 = (1 << 6),      // 2 shorts, 4.12 fixed point
		N2Uv2 = (1 << 7),      // 2 floats
		N2Uv2S2 = (1 << 8),      // 2 shorts, 4.12 fixed point
		N2Uv3 = (1 << 9),      // 2 floats
		N2Uv3S2 = (1 << 10),     // 2 shorts, 4.12 fixed point
		N2Color = (1 << 11),     // 4 floats
		N2ColorUB4N = (1 << 12),     // 4 unsigned bytes, normalized
		N2Tangent = (1 << 13),     // 3 floats
		N2TangentB4N = (1 << 14),     // 4 unsigned bytes, normalized
		N2Binormal = (1 << 15),     // 3 floats
		N2BinormalB4N = (1 << 16),     // 4 unsigned bytes, normalized
		N2Weights = (1 << 17),     // 4 floats
		N2WeightsUB4N = (1 << 18),     // 4 unsigned bytes, normalized
		N2JIndices = (1 << 19),     // 4 floats
		N2JIndicesUB4 = (1 << 20),     // 4 unsigned bytes

		N2NumVertexComponents = 21,
		N2AllComponents = ((1 << N2NumVertexComponents) - 1),
	};

	struct Vertex
	{
		float pos[3];
		float uv[2];
		float normal[3];
		Math::vec4 weights;
		Math::vec4 jIndices;
	};

	bool loadMeshFromFile(const char* filename);

	void setupMesh();

	void draw();
	void* getMesh();
	void* getIndices();

	CharacterSkeleton skeleton;

private:
	GLuint vao[1];
	GLuint ib[1];
	GLuint vbo[1];

	void* mesh;
	void* indices;

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

	std::vector<VertexComponent> vertexComponents;
};

}//namespace Render