#include "config.h"
#include <algorithm>
#include <sstream>
#include "meshresource.h"
#include <cstring>
#include "nvx2fileformatstructs.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

	bool MeshResource::loadMesh(const char* filename)
	{
		const aiScene* scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_Fast);

		if (scene == nullptr)
		{
			printf("ERROR: Could not load mesh!\n");
			assert(false);
			return false;
		}

		this->filePath = filename;
		this->name = filename;

		//Start by checking our vertex width and number of vertices so that we can reserve memory
		//We use the first mesh for determining our vertexlayout and vertexwidth. 
		//TODO: This ^ might be a problem and should probably be looked over later

		//TODO: Check performance for having each primitivegroup have their own indexbuffer but shared VBO, contra having it like now (one single IBO and VBO and them just having offset and length for their own IB segment)

		//All of this is quite the preprocessing step and MUST be moved to a preprocessing program, say a batcher.
		aiMesh* mesh = scene->mMeshes[0];
		
		this->numVertices = 0;
		this->numIndices = 0;

		this->vertexWidth = 0;
		this->vertexDataSize = 0;
		
		//Reserve memory for our primitivegroups
		this->primitiveGroups.Reserve(scene->mNumMeshes);
		
		for (size_t i = 0; i < scene->mNumMeshes; i++)
			this->primitiveGroups.Append(PrimitiveGroup());

		if (mesh->HasPositions())
		{
			VertexComponent::SemanticName sem = VertexComponent::Position;
			VertexComponent::Format fmt = VertexComponent::Float3;
			this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
			this->vertexWidth += 3;
		}
		if (mesh->HasNormals())
		{
			VertexComponent::SemanticName sem = VertexComponent::Normal;
			VertexComponent::Format fmt = VertexComponent::Float3;
			this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
			this->vertexWidth += 3;
		}
		if (mesh->HasTextureCoords(0))
		{
			//TODO: Check how many components texcoords have so that we can load 3d textures too.
			//if (mesh->mNumUVComponents == 2)
			VertexComponent::SemanticName sem = VertexComponent::TexCoord1;
			VertexComponent::Format fmt = VertexComponent::Float2;
			this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
			this->vertexWidth += 2;
		}
		//HACK: Our shaders don't support this yet, so we just ignore it.
		if (mesh->HasTextureCoords(1))
		{
			VertexComponent::SemanticName sem = VertexComponent::TexCoord2;
			VertexComponent::Format fmt = VertexComponent::Float2;
			this->vertexComponents.Append(VertexComponent(sem, 1, fmt));
			this->vertexWidth += 2;
		}
		if (mesh->HasTangentsAndBitangents())
		{
			VertexComponent::SemanticName sem = VertexComponent::Tangent;
			VertexComponent::Format fmt = VertexComponent::Float3;
			this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
			this->vertexWidth += 3;
		
			sem = VertexComponent::Binormal;
			fmt = VertexComponent::Float3;
			this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
			this->vertexWidth += 3;
		}
		if (mesh->HasVertexColors(0))
		{
			VertexComponent::SemanticName sem = VertexComponent::Color;
			VertexComponent::Format fmt = VertexComponent::Float3;
			this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
			this->vertexWidth += 3;
		}

		//Sort to make sure that our components are in the correct order when we construct our vertexbuffer
		this->vertexComponents.Sort();

		//Count numverices and numindices
		//Set size and name for our primitive groups
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			//Add to total amount of vertices
			this->numVertices += scene->mMeshes[i]->mNumVertices;

			this->primitiveGroups[i].name = scene->mMeshes[i]->mName.C_Str();
			//this->primitiveGroups[i].numVertices = scene->mMeshes[i]->mNumVertices;
			//this->primitiveGroups[i].vertexDataSize = this->primitiveGroups[i].numVertices * this->vertexWidth * sizeof(GLfloat);

			this->primitiveGroups[i].surfaceIndex = scene->mMeshes[i]->mMaterialIndex - 1;

			this->primitiveGroups[i].numIndices = 0;

			//count indices
			for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
			{
				this->numIndices += scene->mMeshes[i]->mFaces[j].mNumIndices;
				this->primitiveGroups[i].numIndices += scene->mMeshes[i]->mFaces[j].mNumIndices;
			}

			this->primitiveGroups[i].indexDataSize = this->primitiveGroups[i].numIndices * sizeof(int);
		}
		

		this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(GLfloat);
		this->indexDataSize = this->numIndices * sizeof(int);

		this->mesh = new byte[this->vertexDataSize];
		this->indices = new byte[this->indexDataSize];

		//Point to the first element in our indexbuffer
		this->indexDataPtr = this->indices;

		//Construct our indexbuffer
		{
			const byte triangleByteSize = 3 * sizeof(int);

			size_t sumNumVertices = 0;
			for (size_t i = 0; i < scene->mNumMeshes; ++i)
			{
				mesh = scene->mMeshes[i];

				for (size_t j = 0; j < mesh->mNumFaces; ++j)
				{
					if (mesh->mFaces[j].mNumIndices == 3)
					{
						//Triangles. Just append everything to the indexbuffer
						memcpy(this->indexDataPtr, mesh->mFaces[j].mIndices, triangleByteSize);
						
						//We need to compensate for appending them to the same indexbuffer. We do this by increasing every index with the sum of the number of vertices that are in the meshes before it
						*(GLuint*)indexDataPtr = *(GLuint*)this->indexDataPtr + (GLuint)sumNumVertices;
						*((GLuint*)indexDataPtr + 1) = *((GLuint*)indexDataPtr + 1) + (GLuint)sumNumVertices;
						*((GLuint*)indexDataPtr + 2) = *((GLuint*)indexDataPtr + 2) + (GLuint)sumNumVertices;

						this->indexDataPtr = (byte*)indexDataPtr + triangleByteSize;
					}
					else if (mesh->mFaces[j].mNumIndices == 2)
					{
						printf("ERROR: Meshloader does not support lines within meshes!");
						assert(false);
					}
					else if (mesh->mFaces[j].mNumIndices == 4)
					{
						printf("ERROR: Meshloader does not support quads within meshes!");
						assert(false);
					}
				}
				sumNumVertices += mesh->mNumVertices;
			}
		}
		this->vertexDataPtr = this->mesh;

		//Start constructing boundingbox.
		this->bbox.begin_extend();

		//Construct our vertexbuffer
		for (size_t a = 0; a < scene->mNumMeshes; ++a)
		{
			mesh = scene->mMeshes[a];

			for (size_t i = 0; i < mesh->mNumVertices; ++i)
			{
				for (uint j = 0; j < this->vertexComponents.Size(); j++)
				{
					size_t byteSize = this->vertexComponents[j].GetByteSize();

					switch (this->vertexComponents[j].GetSemanticName())
					{
					case VertexComponent::SemanticName::Position:
					{
						this->bbox.extend(Math::point(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
						memcpy(this->vertexDataPtr, &mesh->mVertices[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Normal:
					{
						memcpy(this->vertexDataPtr, &mesh->mNormals[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::TexCoord1:
					{
						memcpy(this->vertexDataPtr, &mesh->mTextureCoords[0][i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::TexCoord2:
					{
						memcpy(this->vertexDataPtr, &mesh->mTextureCoords[1][i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Tangent:
					{
						memcpy(this->vertexDataPtr, &mesh->mTangents[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Binormal:
					{
						memcpy(this->vertexDataPtr, &mesh->mBitangents[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Color:
					{
						memcpy(this->vertexDataPtr, &mesh->mColors[0][i], byteSize);
						break;
					}
					}

					this->vertexDataPtr = (byte*)vertexDataPtr + byteSize;
				}
			}
		}
		
		this->bbox.end_extend();

		//this->primitiveGroups[0].vertices = this->mesh;
		this->primitiveGroups[0].indexOffset = 0;

		size_t offset = 0;

		for (size_t i = 0; i < scene->mNumMeshes; ++i)
		{
			//this->primitiveGroups[i].vertices = (size_t*)this->mesh + this->primitiveGroups[i - 1].vertexDataSize;
			this->primitiveGroups[i].indexOffset = offset;
			offset += this->primitiveGroups[i].indexDataSize;
		}

		this->SetupVertexBuffer();
		this->SetupIndexBuffer();

		//delete scene
		aiReleaseImport(scene);
				

		return true;
	}

	bool MeshResource::loadMeshFromFile(const char* filename)
	{
		FILE * file;
		uchar* buffer;
		size_t bufferSize;
		size_t result;

		file = fopen(filename, "rb");

		assert(file != NULL);

		this->N2File = true;

		this->name = filename;
		this->filePath = filename;

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

		//If we've loaded a Nebula file we need to do some extra stuff
		if (N2File)
		{
			uchar* ptr = (uchar*)this->vertexDataPtr;

			//setup mesh to be the size of our vertex data block
			this->mesh = new uchar[vertexDataSize];

			//Copy vertexdata!
			memcpy(this->mesh, ptr, vertexDataSize);
		}

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
		//If we've loaded a Nebula file we need to do some extra stuff
		if (N2File)
		{
			uchar* ptr = (uchar*)this->indexDataPtr;

			//setup indices to be the size of our index data block
			this->indices = new uchar[indexDataSize];

			//Copy indexdata!
			memcpy(this->indices, ptr, indexDataSize);
		}

		glGenBuffers(1, this->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexDataSize, indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
	}

	void MeshResource::Unbind()
	{
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void MeshResource::Draw(const unsigned int& primitiveGroup)
	{
		//glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);
		glDrawElements(GL_TRIANGLES, (GLsizei)primitiveGroups[primitiveGroup].numIndices, GL_UNSIGNED_INT, (void*)primitiveGroups[primitiveGroup].indexOffset);
	}

}