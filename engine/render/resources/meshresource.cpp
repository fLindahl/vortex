#include "config.h"
#include <algorithm>
#include <sstream>
#include "meshresource.h"
#include <cstring>
#include <GLFW/glfw3.h>


namespace Render
{

	MeshResource::MeshResource()
	{

	}

	MeshResource::~MeshResource()
	{
		glDeleteVertexArrays(1, &vao[0]);
		glDeleteBuffers(1, &vbo[0]);
		glDeleteBuffers(1, &ib[0]);
	}

	bool MeshResource::loadMeshFromFile(const char* filename)
	{
		Util::Array< unsigned int > vertexIndices, uvIndices, normalIndices;
		Util::Array< Math::Vector3 > temp_vertices;
		Util::Array< Math::Vector2 > temp_uvs;
		Util::Array< Math::Vector3 > temp_normals;

		std::map <unsigned long, Vertex*> indexBitToVertexMap;
		std::map <unsigned int, unsigned int> vertexMemoryAdressToIndex;

		//std::vector< GLfloat > temp_mesh;

		FILE * file = fopen(filename, "r");
		if (file == NULL){
			printf("Impossible to open the file !\n");
			return false;
		}
		int32 index = 0;
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
				temp_vertices.Append(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				GLfloat uv[2];
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				temp_uvs.Append(uv);
			}

			else if (strcmp(lineHeader, "vn") == 0)
			{
				GLfloat normal[3];
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				temp_normals.Append(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

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
				unsigned long long indexBit;
				Vertex* tempVertex;

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
						tempVertex = new Vertex();

						tempVertex->pos = temp_vertices[vertexIndex[u]];
						tempVertex->uv = temp_uvs[uvIndex[u]];
						tempVertex->normal = temp_normals[normalIndex[u]];

						indexBitToVertexMap[indexBit] = tempVertex;

						mesh.Append(*tempVertex);

						//Use the memory adress as a number to quickly be able to lookup the vertex index
						vertexMemoryAdressToIndex[(uintptr_t)tempVertex] = mesh.Size() - 1;
					}

					indices.Append(vertexMemoryAdressToIndex[(uintptr_t)tempVertex]);
				}
			}
		}
		return true;
	}

	Util::Array < MeshResource::Vertex >& MeshResource::getMesh()
	{
		return this->mesh;
	}
	Util::Array < GLuint >& MeshResource::getIndices()
	{
		return this->indices;
	}

	void MeshResource::setupMesh()
	{
		glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object  
		glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it  

		glGenBuffers(1, this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, mesh.Size() * sizeof(Vertex), &mesh[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(float32) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(float32) * 5));

		glGenBuffers(1, this->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshResource::createQuad()
	{
		MeshResource::Vertex vert1 = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::Vertex vert2 = { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::Vertex vert3 = { { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::Vertex vert4 = { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		mesh.Append(vert1);
		mesh.Append(vert2);
		mesh.Append(vert3);
		mesh.Append(vert4);

		indices.Append(0);
		indices.Append(1);
		indices.Append(2);
		indices.Append(0);
		indices.Append(2);
		indices.Append(3);
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
		glDrawElements(GL_TRIANGLES, indices.Size(), GL_UNSIGNED_INT, NULL);
	}

}