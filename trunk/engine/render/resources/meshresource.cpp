#include "config.h"
#include <algorithm>
#include <sstream>
#include "meshresource.h"
#include <cstring>
#include "nvx2fileformatstructs.h"

namespace Render
{
	MeshResource::MeshResource()
	{
		this->name = "N/A";
		rawMode = false;
	}

	MeshResource::~MeshResource()
	{
		glDeleteVertexArrays(1, &vao[0]);
		glDeleteBuffers(1, &vbo[0]);
		glDeleteBuffers(1, &ib[0]);
	}

	bool MeshResource::loadMeshFromFile(const char* filename)
	{
		FILE * file;
		uchar* buffer;
		size_t bufferSize;
		size_t result;

		file = fopen(filename, "rb");

		assert(file != NULL);

		this->name = filename;

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
		this->mapPtr = new uchar[10];

		this->mapPtr = buffer;

		// read data
		this->ReadHeaderData();
		this->ReadPrimitiveGroups();
		this->SetupVertices();
		if (!this->rawMode)
		{
			this->SetupVertexBuffer();
			this->SetupIndexBuffer();
		}

		//TODO: Shouldn't we delete the buffer?


		return true;
	}

	void MeshResource::ReadHeaderData()
	{
		struct Nvx2Header* header = (struct Nvx2Header*) this->mapPtr;
		header->numIndices *= 3; // header holds number of tris, not indices

		this->numGroups = header->numGroups;
		this->numVertices = header->numVertices;
		this->vertexWidth = header->vertexWidth;
		this->numIndices = header->numIndices;
		this->numEdges = header->numEdges;
		this->vertexComponentMask = header->vertexComponentMask;
		this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
		this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
		this->indexDataSize = this->numIndices * sizeof(int);

		this->groupDataPtr = header + 1;
		this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
		this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;
	}

	void MeshResource::ReadPrimitiveGroups()
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

	void MeshResource::SetupVertices()
	{
		size_t i;
		for (i = 0; i < N2NumVertexComponents; i++)
		{
			VertexComponent::SemanticName sem;
			VertexComponent::Format fmt;
			uint index = 0;
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
				this->vertexComponents.Append(VertexComponent(sem, index, fmt));
			}
		}
	}

	void MeshResource::SetupVertexBuffer()
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


		GLbyte* offset = NULL;

		for (uint i = 0; i < this->vertexComponents.Size(); i++)
		{

			attributeIndex = (uint)this->vertexComponents[i].GetSemanticName();

			byteSize = this->vertexComponents[i].GetByteSize();
			format = this->vertexComponents[i].GetFormat();

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

			offset += byteSize;
		}

	}

	void MeshResource::SetupIndexBuffer()
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

	bool MeshResource::loadMeshFromOBJ(const char* filename)
	{
		Util::Array< Util::Array<GLfloat> > temp_vertices;
		Util::Array< Util::Array<GLfloat> > temp_uvs;
		Util::Array< Util::Array<GLfloat> > temp_normals;

		std::map<unsigned long, OBJVertex*> indexBitToVertexMap;
		std::map<unsigned int, unsigned int> vertexMemoryAdressToIndex;

		FILE * file = fopen(filename, "r");
		if (file == NULL){
			printf("Impossible to open the file !\n");
			return false;
		}

		this->name = filename;

		index_t index = 0;

		//Start constructing boundingbox.
		this->bbox.begin_extend();

		while (true)
		{
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0)
			{
				GLfloat vertex[3];
				fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				Util::Array<GLfloat> a;
				a.Append(vertex[0]);
				a.Append(vertex[1]);
				a.Append(vertex[2]);
				temp_vertices.Append(a);

				this->bbox.extend(Math::point(vertex[0], vertex[1], vertex[2]));
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				GLfloat uv[2];
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				Util::Array<GLfloat> a;
				a.Append(uv[0]);
				a.Append(uv[1]);
				temp_uvs.Append(a);
			}

			else if (strcmp(lineHeader, "vn") == 0)
			{
				GLfloat normal[3];
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				Util::Array<GLfloat> a;
				a.Append(normal[0]);
				a.Append(normal[1]);
				a.Append(normal[2]);
				temp_normals.Append(a);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

				int matches = fscanf(file, "%i/%i/%i %i/%i/%i %i/%i/%i\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}

				//subtract 1 from indices because we want to use 0 indexing.
				for (int idx = 0; idx < 3; idx++)
				{
					vertexIndex[idx] -= 1;
					uvIndex[idx] -= 1;
					normalIndex[idx] -= 1;
				}

				// for each vertex in face
				index_t indexBit;
				OBJVertex* tempVertex;

				for (GLuint u = 0; u < 3; u++)
				{
					//Make sure we dont duplicate any vertices
					indexBit = ((vertexIndex[u]) + (uvIndex[u] << 21) + (normalIndex[u] << 42));

					if (indexBitToVertexMap.count(indexBit))
					{
						tempVertex = indexBitToVertexMap[indexBit];
					}
					else
					{
						//Create a new vertex
						tempVertex = new OBJVertex();

						tempVertex->pos[0] = temp_vertices[vertexIndex[u]][0];
						tempVertex->pos[1] = temp_vertices[vertexIndex[u]][1];
						tempVertex->pos[2] = temp_vertices[vertexIndex[u]][2];

						tempVertex->uv[0] = temp_uvs[uvIndex[u]][0];
						tempVertex->uv[1] = temp_uvs[uvIndex[u]][1];

						tempVertex->normal[0] = temp_normals[normalIndex[u]][0];
						tempVertex->normal[1] = temp_normals[normalIndex[u]][1];
						tempVertex->normal[2] = temp_normals[normalIndex[u]][2];

						indexBitToVertexMap[indexBit] = tempVertex;

						OBJvertexBuffer.Append(*tempVertex);

						//Use the memory adress as a number to quickly be able to lookup the vertex index
						vertexMemoryAdressToIndex[(uintptr_t)tempVertex] = OBJvertexBuffer.Size() - 1;
					}

					OBJindexBuffer.Append(vertexMemoryAdressToIndex[(uintptr_t)tempVertex]);
				}
			}
		}

		this->bbox.end_extend();

		this->numIndices = OBJindexBuffer.Size();

		glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object  
		glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it  

		glGenBuffers(1, this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, OBJvertexBuffer.Size() * sizeof(OBJVertex), &OBJvertexBuffer[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 5));

		glGenBuffers(1, this->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), &OBJindexBuffer[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

	void* MeshResource::getMesh()
	{
		return this->mesh;
	}
	void* MeshResource::getIndices()
	{
		return this->indices;
	}
	
	void MeshResource::Bind()
	{
		glBindVertexArray(vao[0]); // Bind our Vertex Array Object 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
	}

	void MeshResource::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void MeshResource::Draw()
	{
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	}

}