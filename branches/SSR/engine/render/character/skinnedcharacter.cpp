#include "config.h"
#include <algorithm>
#include <sstream>
#include "skinnedcharacter.h"
#include <cstring>
#include "foundation/math/vector4.h"
#include <GLFW/glfw3.h>
#include "render/resources/meshfileformatstructs.h"

namespace Render
{

SkinnedCharacter::SkinnedCharacter()
{
	rawMode = false;
	skeleton = CharacterSkeleton();
}

SkinnedCharacter::~SkinnedCharacter()
{
	glDeleteVertexArrays(1, &vao[0]);
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &ib[0]);
}

bool SkinnedCharacter::loadMeshFromFile(const char* filename)
{
	FILE * file;
	size_t bufferSize;
	uchar* buffer;
	size_t result;

	file = fopen(filename, "rb");

	assert(file != NULL);

	// obtain file size:
	fseek(file, 0, SEEK_END);
	bufferSize = ftell(file);
	rewind(file);

	// allocate memory to contain the whole file:
	buffer = new uchar[bufferSize];

	//assert(buffer == NULL);

	// copy the file into the buffer:
	result = fread(buffer, 1, bufferSize, file);

	//assert(result != bufferSize);
	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose(file);

	//LETS GO

	mapPtr = (uchar*)buffer;

	// read data
	this->ReadHeaderData();
	this->ReadPrimitiveGroups();
	this->SetupVertices();
	if (!this->rawMode)
	{
		this->SetupVertexBuffer();
		this->SetupIndexBuffer();
	}

	return true;
}

void SkinnedCharacter::ReadHeaderData()
{
	struct MeshFileHeader* header = (struct MeshFileHeader*) this->mapPtr;
	header->numIndices *= 3; // header holds number of tris, not indices

	this->numGroups = header->numGroups;
	this->numVertices = header->numVertices;
	this->vertexWidth = header->vertexWidth;
	this->numIndices = header->numIndices;
	this->vertexComponentMask = header->vertexComponentMask;
	this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
	this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
	this->indexDataSize = this->numIndices * sizeof(int);

	this->groupDataPtr = header + 1;
	this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
	this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;
}

void SkinnedCharacter::ReadPrimitiveGroups()
{
	/*Nvx2Group* group = (Nvx2Group*) this->groupDataPtr;
	size_t i;
	for (i = 0; i < (size_t)this->numGroups; i++)
	{
	// setup a primitive group object
	PrimitiveGroup primGroup;
	//primGroup.SetBaseVertex(group->firstVertex);
	primGroup.SetNumVertices(group->numVertices);
	primGroup.SetBaseIndex(group->firstTriangle * 3);
	primGroup.SetNumIndices(group->numTriangles * 3);
	primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
	this->primGroups.Append(primGroup);

	// set top next group
	group++;
	}*/
}

void SkinnedCharacter::SetupVertices()
{
	size_t i;
	for (i = 0; i < N2NumVertexComponents; i++)
	{
		VertexComponent::SemanticName sem;
		VertexComponent::Format fmt;
		size_t index = 0;
		if (vertexComponentMask & (1 << i))
		{
			switch (1 << i)
			{
			case N2Coord:        sem = VertexComponent::Position;     fmt = VertexComponent::Float3; break;
			case N2Normal:       sem = VertexComponent::Normal;       fmt = VertexComponent::Float3; break;
			case N2NormalB4N:    sem = VertexComponent::Normal;       fmt = VertexComponent::Byte4N; break;
			case N2Uv0:          sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Float2; index = 0; break;
			case N2Uv0S2:        sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Short2; index = 0; break;
			case N2Uv1:          sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Float2; index = 1; break;
			case N2Uv1S2:        sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Short2; index = 1; break;
			case N2Color:        sem = VertexComponent::Color;        fmt = VertexComponent::Float4; break;
			case N2ColorUB4N:    sem = VertexComponent::Color;        fmt = VertexComponent::UByte4N; break;
			case N2Tangent:      sem = VertexComponent::Tangent;      fmt = VertexComponent::Float3; break;
			case N2TangentB4N:   sem = VertexComponent::Tangent;      fmt = VertexComponent::Byte4N; break;
			case N2Binormal:     sem = VertexComponent::Binormal;     fmt = VertexComponent::Float3; break;
			case N2BinormalB4N:  sem = VertexComponent::Binormal;     fmt = VertexComponent::Byte4N; break;
			case N2Weights:      sem = VertexComponent::SkinWeights;  fmt = VertexComponent::Float4; break;
			case N2WeightsUB4N:  sem = VertexComponent::SkinWeights;  fmt = VertexComponent::UByte4N; break;
			case N2JIndices:     sem = VertexComponent::SkinJIndices; fmt = VertexComponent::Float4; break;
			case N2JIndicesUB4:  sem = VertexComponent::SkinJIndices; fmt = VertexComponent::UByte4; break;
			default:
				std::printf("Invalid Nebula2 VertexComponent in Nvx2StreamReader::SetupVertexComponents");
				sem = VertexComponent::Position;
				fmt = VertexComponent::Float3;
				break;
			}
			this->vertexComponents.push_back(VertexComponent(sem, (uint)index, fmt));
		}
	}
}

void SkinnedCharacter::SetupVertexBuffer()
{
	// setup new vertex buffer
	//this->mesh.reserve(this->numVertices);

	uchar* ptr = (uchar*)this->vertexDataPtr;

	//setup mesh to be the size of our vertex data block
	this->mesh = new uchar[vertexDataSize];

	//Copy vertexdata!
	memcpy(this->mesh, ptr, vertexDataSize);

	glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object  
	glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it  

	glGenBuffers(1, this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, this->vertexDataSize, mesh, GL_STATIC_DRAW);


	uint attributeIndex = 0;
	size_t byteSize = 0;
	VertexComponent::Format format;
	GLenum type;
	GLboolean normalized;
	GLint components;


	GLint* offset = NULL;

	for (uint i = 0; i < this->vertexComponents.size(); i++)
	{

		attributeIndex = (int)this->vertexComponents[i].GetSemanticName();

		byteSize = this->vertexComponents[i].GetByteSize();
		format = this->vertexComponents[i].GetFormat();

		//Skip everything but positions, normals, uvs, skinweights and joint indices
		if (attributeIndex == 0 || attributeIndex == 1 || attributeIndex == 2 || attributeIndex == 7 || attributeIndex == 8)
		{
			glEnableVertexAttribArray(attributeIndex);

			switch (format)
			{
			case VertexComponent::Float:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 1;	break;
			case VertexComponent::Float2:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 2;	break;
			case VertexComponent::Float3:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 3;	break;
			case VertexComponent::Float4:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 4;	break;
			case VertexComponent::UByte4:	type = GL_UNSIGNED_BYTE;	normalized = GL_FALSE;	components = 4;	break;
			case VertexComponent::Byte4:	type = GL_BYTE;			normalized = GL_FALSE;	components = 4;	break;
			case VertexComponent::Short2:	type = GL_SHORT;		normalized = GL_FALSE;	components = 2;	break;
			case VertexComponent::Short4:	type = GL_SHORT;		normalized = GL_FALSE;	components = 4;	break;
			case VertexComponent::UByte4N:	type = GL_UNSIGNED_BYTE;	normalized = GL_TRUE;	components = 4;	break;
			case VertexComponent::Byte4N:	type = GL_BYTE;			normalized = GL_TRUE;	components = 4;	break;
			case VertexComponent::Short2N:	type = GL_SHORT;		normalized = GL_TRUE;	components = 2;	break;
			case VertexComponent::Short4N:	type = GL_SHORT;		normalized = GL_TRUE;	components = 4;	break;
			}

			glVertexAttribPointer(attributeIndex, components, type, normalized, sizeof(GLfloat) * this->vertexWidth, (GLvoid*)offset);
		}

		offset += byteSize;
	}


}

void SkinnedCharacter::SetupIndexBuffer()
{
	uchar* ptr = (uchar*)this->indexDataPtr;

	//setup indices to be the size of our index data block
	this->indices = new uchar[indexDataSize];

	//Copy indexdata!
	memcpy(this->indices, ptr, indexDataSize);

	glGenBuffers(1, this->ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexDataSize, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void* SkinnedCharacter::getMesh()
{
	return this->mesh;
}
void* SkinnedCharacter::getIndices()
{
	return this->indices;
}

void SkinnedCharacter::setupMesh()
{
	/*
	glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object
	glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(1, this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), &mesh[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(float32) * 5));

	glGenBuffers(1, this->ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
}

void SkinnedCharacter::draw()
{
	glBindVertexArray(vao[0]); // Bind our Vertex Array Object 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
	glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind our Vertex Array Object  

}
}//namespace Render