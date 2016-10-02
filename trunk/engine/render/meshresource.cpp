#include "config.h"
#include <algorithm>
#include <sstream>
#include "meshresource.h"
#include <cstring>
#include "foundation/math/vector4.h"
//#include <sys/resource.h>
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
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< Math::Vector3 > temp_vertices;
		std::vector< Math::Vector2 > temp_uvs;
		std::vector< Math::Vector3 > temp_normals;

		std::map <unsigned long, Vertex*> indexBitToVertexMap;
				
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
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				GLfloat uv[2];
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				temp_uvs.push_back(uv);
			}

			else if (strcmp(lineHeader, "vn") == 0)
			{
				GLfloat normal[3];
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				temp_normals.push_back(normal);
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
					
					if(indexBitToVertexMap.count(indexBit))
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
						
						mesh.push_back(*tempVertex);
						
						//Use the memory adress as a number to quickly be able to lookup the vertex index
						vertexMemoryAdressToIndex[(uintptr_t)tempVertex] = mesh.size() - 1;
					}
					
					indices.push_back(vertexMemoryAdressToIndex[(uintptr_t)tempVertex]);
				}
			}
		}
		return true;
	}

	bool MeshResource::generateHalfEdgeMeshFromFile(const char* filename)
	{
	  
	    double time = glfwGetTime();
	  
		//Hardcoded for CUBE
		this->vertices = Array<Vertex>(64 * pow(10, 6));
		this->faces = Array<Face>(32 * pow(10, 6));
		this->edges = Array<HalfEdge>(105 * pow(10, 6));

		  
	    time = glfwGetTime() - time;
	    printf("Memory Allocation Time : %f s\n", time);
		
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< Math::Vector3 > temp_vertices;
		std::vector< Math::Vector2 > temp_uvs;
		std::vector< Math::Vector3 > temp_normals;
		
		std::map <uint64_t, unsigned int> indexBitToVertexMap;
		std::map <std::pair<unsigned int, unsigned int>, HalfEdge* > edgeMap;		
		
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
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				GLfloat uv[2];
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				temp_uvs.push_back(uv);
			}

			else if (strcmp(lineHeader, "vn") == 0)
			{
				GLfloat normal[3];
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
				
				if (!(matches == 9 || matches == 12)){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				
				// Is it 4 or 3 vertices per face
				const int verticesPerFace = 4 - (matches % 4);
				
				
				//subtract 1 from indices because we want to use 0 indexing.
				for (int idx = 0; idx < verticesPerFace; idx++)
				{
					vertexIndex[idx] -= 1;
					uvIndex[idx] -= 1;
					normalIndex[idx] -= 1;
				}

				//HACK: 4 should be verticesPerFace
				HalfEdge* tempEdges[4];

				Face* face = &this->faces.NewElement(); //new Face();
				
				// for each vertex in face
				uint64_t indexBit;
				Vertex* tempVertex;
				size_t currentIndex;
				
				for (GLuint u = 0; u < verticesPerFace; u++)
				{
					//Make sure we dont duplicate any vertices
					indexBit = ((vertexIndex[u]) + (uvIndex[u] << 21) + (normalIndex[u] << 42));
					
					if(indexBitToVertexMap.count(indexBit))
					{
						currentIndex = indexBitToVertexMap[indexBit];
					}
					else
					{
						//tempVertex = new Vertex();
						tempVertex = &this->vertices.NewElement(currentIndex);
						
						tempVertex->pos = temp_vertices[vertexIndex[u]];
						tempVertex->uv = temp_uvs[uvIndex[u]];
						tempVertex->normal = temp_normals[normalIndex[u]];
						
						//vertices.push_back(tempVertex);
						
						indexBitToVertexMap[indexBit] = currentIndex; //vertices.size() - 1
						//currentIndex = vertices.size() - 1;
						
					}
					
					HalfEdge* edge = &this->edges.NewElement(); //new HalfEdge;
					tempEdges[u] = edge;
					  
					edge->vertex = currentIndex;
					
					tempVertex->halfedge = edge;
					
					edge->face = face;
					
				}
				face->edge = tempEdges[0];
				
				//faces.push_back(face);
				
				//Make each edge correspond to a pair of vertices
				for(GLuint u = 0; u < verticesPerFace; u++)
				{
					const GLuint v = u + 1;
					int vIdx1 = tempEdges[u]->vertex;
					int vIdx2 = tempEdges[(v > verticesPerFace - 1 ? 0 : v)]->vertex;
					
					edgeMap[std::pair<unsigned int, unsigned int>(vIdx1, vIdx2)] = tempEdges[u];
				}
				
				//For each edge, set next edge and opposite edge
				for (GLuint u = 0; u < verticesPerFace; u++)
				{
					const int v = (u + 1 > verticesPerFace - 1 ? 0 : u + 1);
					
					tempEdges[u]->nextHalfEdge = tempEdges[(v)];
					
					int vIdx1 = tempEdges[u]->vertex;
					int vIdx2 = tempEdges[v]->vertex;
					
					if (edgeMap.find(std::pair<unsigned int, unsigned int>(vIdx2, vIdx1)) != edgeMap.end())
					{
						edgeMap[std::pair<unsigned int, unsigned int>(vIdx1, vIdx2)]->oppositeHalfEdge = edgeMap[std::pair<unsigned int, unsigned int>(vIdx2, vIdx1)];
						edgeMap[std::pair<unsigned int, unsigned int>(vIdx2, vIdx1)]->oppositeHalfEdge = edgeMap[std::pair<unsigned int, unsigned int>(vIdx1, vIdx2)];
					}  
				}
				
			}
			
		}
		return true;
	}

	bool MeshResource::loadMeshFromHalfEdgeMesh()
	{
		for (int i = 0; i < vertices.size(); i++)
		{
		  mesh.push_back(vertices[i]);
		}
	  
		for (int i = 0; i < faces.size(); i++)
		{
			Face* F = &faces[i];
			
			HalfEdge* firstHalfEdge = F->edge;
			HalfEdge* currentHalfEdge = F->edge;
			
			//How many triangles are we going to make?
			int nVerts = 1;
			unsigned int firstVert = currentHalfEdge->vertex;
			unsigned int currentVert;
			
			do
			{
			  currentHalfEdge = currentHalfEdge->nextHalfEdge;
			  currentVert = currentHalfEdge->vertex;
			  nVerts++;      
			  
			} while (currentVert != firstVert);
				
			int nTriangles = nVerts - 2;
			
			for (int i = 0; i < nTriangles; i++)
			{
				for (int nEdgesTraversed = 0; nEdgesTraversed < 2; nEdgesTraversed++)
				{
					unsigned int vert = currentHalfEdge->vertex;
					currentHalfEdge = currentHalfEdge->nextHalfEdge;
					indices.push_back(vert);
				}
				
				unsigned int vert = currentHalfEdge->vertex;
				indices.push_back(vert);
			}
		}
		return true;
	}
	
void MeshResource::Subdivide(const int& amount)
{

// 	uint64_t newFaceCapacity = faces.size() * pow(4, amount);
// 	
// 	//TEMP 16m cappacity
// 	uint64_t newVertexCapacity = 16 * pow(10, 6);
// 
// 	//TEMP 32m cappacity
// 	uint64_t newEdgeCapacity = 32 * pow(10, 6);
// 	
// 	faces.Resize(newFaceCapacity);
// 	vertices.Resize(newVertexCapacity);
// 	edges.Resize(newEdgeCapacity);   
  

	for(int x = 0; x < amount; x++)
	{
	    double time = glfwGetTime();
	    
	    //First of all we need a copy of all the original vertices
	    std::vector<Vertex> originalVertices;
	    for (int i = 0; i < vertices.size(); i++)
	    {
			Vertex* v = &vertices[i];
	      
			originalVertices.push_back(*v);
			//Set positions to zero so that we can do shit fast!
			v->pos = Math::Vector3();
	    }
	    
	    int facesSize = faces.size();
	    
	    for (int i = 0; i < facesSize; i++)
	    {
			//For each face we add 3 face, 12 edges and possibly up to 4 vertices and calculate all positions right away!
			Face* oldFace = &faces[i];
	      
			//Facepoint
			size_t ifp;
			Vertex* fp = &vertices.NewElement(ifp);
			
			//Vertex* fp = new Vertex();
			
			//vertices.push_back(fp);
			//unsigned int ifp = vertices.size() - 1;	      
	      
			//Calculate facepoint position
			HalfEdge* currentHalfEdge = oldFace->edge;
	      
			//Add 4 new edges for the old face
			HalfEdge* E1 = currentHalfEdge;
			HalfEdge* E2 = currentHalfEdge->nextHalfEdge;
			HalfEdge* E3 = currentHalfEdge->nextHalfEdge->nextHalfEdge;
			HalfEdge* E4 = currentHalfEdge->nextHalfEdge->nextHalfEdge->nextHalfEdge;
	      
			HalfEdge* NE1 = &edges.NewElement();//new HalfEdge();
			HalfEdge* NE2 = &edges.NewElement();
			HalfEdge* NE3 = &edges.NewElement();
			HalfEdge* NE4 = &edges.NewElement();
									   
			HalfEdge* FE1 = &edges.NewElement();
			HalfEdge* FE2 = &edges.NewElement();
			HalfEdge* FE3 = &edges.NewElement();
			HalfEdge* FE4 = &edges.NewElement();
			HalfEdge* FE5 = &edges.NewElement();
			HalfEdge* FE6 = &edges.NewElement();
			HalfEdge* FE7 = &edges.NewElement();
			HalfEdge* FE8 = &edges.NewElement();

			size_t iOV1 = E1->vertex;
			Vertex* OV1 = &vertices[iOV1];
			size_t iOV2 = E2->vertex;
			Vertex* OV2 = &vertices[iOV2];
			size_t iOV3 = E3->vertex;
			Vertex* OV3 = &vertices[iOV3];
			size_t iOV4 = E4->vertex;
			Vertex* OV4 = &vertices[iOV4];
	      
			Vertex* NV1;
			size_t iNV1;
			Vertex* NV2;
			size_t iNV2;
			Vertex* NV3;
			size_t iNV3;
			Vertex* NV4;
			size_t iNV4;
	      
			Face* NF1 = &faces.NewElement(); //new Face();
			Face* NF2 = &faces.NewElement();
			Face* NF3 = &faces.NewElement();
	      
			//faces.push_back(NF1);
			//faces.push_back(NF2);
			//faces.push_back(NF3);
	      
			NF1->edge = E1;
			NF2->edge = E2;
			NF3->edge = E3;
			oldFace->edge = E4;
	      
			//new face 1
			E1->nextHalfEdge = FE1;
			FE1->nextHalfEdge = FE2;
			FE2->nextHalfEdge = NE4;
			NE4->nextHalfEdge = E1;
			E1->face = NF1;
			FE1->face = NF1;
			FE2->face = NF1;
			NE4->face = NF1;
	      
	      
			//new face 2
			E2->nextHalfEdge = FE3;
			FE3->nextHalfEdge = FE4;
			FE4->nextHalfEdge = NE1;
			NE1->nextHalfEdge = E2;
			E2->face = NF2;
			FE3->face = NF2;
			FE4->face = NF2;
			NE1->face = NF2;
	      
	      
			//new face 3
			E3->nextHalfEdge = FE5;
			FE5->nextHalfEdge = FE6;
			FE6->nextHalfEdge = NE2;
			NE2->nextHalfEdge = E3;
			E3->face = NF3;
			FE5->face = NF3;
			FE6->face = NF3;
			NE2->face = NF3;
	      
			//old face
			E4->nextHalfEdge = FE7;
			FE7->nextHalfEdge = FE8;
			FE8->nextHalfEdge = NE3;
			NE3->nextHalfEdge = E4;
			E4->face = oldFace;
			FE7->face = oldFace;
			FE8->face = oldFace;
			NE3->face = oldFace;
	      
			//Set all opposite edges!
			FE1->oppositeHalfEdge = FE4;
			FE4->oppositeHalfEdge = FE1;
	      
			FE3->oppositeHalfEdge = FE6;
			FE6->oppositeHalfEdge = FE3;
	      
			FE5->oppositeHalfEdge = FE8;
			FE8->oppositeHalfEdge = FE5;
	      
			FE7->oppositeHalfEdge = FE2;
			FE2->oppositeHalfEdge = FE7;
	      
			if(E1->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge != nullptr)
			{
				NV1 = &vertices.NewElement(iNV1);
				//NV1 = new Vertex();
				//vertices.push_back(NV1);
				//iNV1 = vertices.size() - 1;
				NV1->halfedge = FE1;
			}
			else
			{
				iNV1 = E1->oppositeHalfEdge->nextHalfEdge->vertex;
				NV1 = &vertices[iNV1];
			}
	      
			if(E2->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge != nullptr)
			{
				NV2 = &vertices.NewElement(iNV2);
				//NV2 = new Vertex();
				//vertices.push_back(NV2);
				//iNV2 = vertices.size() - 1;
				NV2->halfedge = FE3;
			}
			else
			{
				iNV2 = E2->oppositeHalfEdge->nextHalfEdge->vertex;
				NV2 = &vertices[iNV2];
			}
	      
			if(E3->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge != nullptr)
			{
				NV3 = &vertices.NewElement(iNV3);
				//NV3 = new Vertex();
				//vertices.push_back(NV3);
				//iNV3 = vertices.size() - 1;
				NV3->halfedge = FE5;
			}
			else
			{
				iNV3 = E3->oppositeHalfEdge->nextHalfEdge->vertex;
				NV3 = &vertices[iNV3];
			}
	      
			if(E4->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge != nullptr)
			{
				NV4 = &vertices.NewElement(iNV4);
				//NV4 = new Vertex();
				//vertices.push_back(NV4);
				//iNV4 = vertices.size() - 1;
				NV4->halfedge = FE7;
			}
			else
			{
				iNV4 = E4->oppositeHalfEdge->nextHalfEdge->vertex;
				NV4 = &vertices[iNV4];
			}
	      
			fp->halfedge = FE2;
	      
			FE1->vertex = iNV1;
	       
			FE2->vertex = ifp;
	      
			NE4->vertex = iNV4;
	      
			NE1->vertex = iNV1;
	     
			FE3->vertex = iNV2;
	      
			FE4->vertex = ifp;
	      
			NE2->vertex = iNV2;
			FE6->vertex = ifp;
			FE5->vertex = iNV3;
	      
			NE3->vertex = iNV3;
			FE8->vertex = ifp;
			FE7->vertex = iNV4;
	      
	      
			//Set face position
			fp->pos = (originalVertices[iOV1].pos + originalVertices[iOV2].pos + originalVertices[iOV3].pos + originalVertices[iOV4].pos) * 0.25f;
	      
	      
			//Special cases for opposite edges!
			if(E1->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge == nullptr)
			{
				NE1->oppositeHalfEdge = E1->oppositeHalfEdge;
				NE1->oppositeHalfEdge->oppositeHalfEdge = NE1;
		
				E1->oppositeHalfEdge = E1->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge;
				E1->oppositeHalfEdge->oppositeHalfEdge = E1;   
		
				//Add to edgepoint average for both sides
				Vertex* fp2 = &vertices[NE1->oppositeHalfEdge->nextHalfEdge->nextHalfEdge->vertex];
				NV1->pos = (originalVertices[iOV1].pos + originalVertices[iOV2].pos + fp->pos + fp2->pos) * 0.25f;
		
			}
			if(E2->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge == nullptr)
			{
				NE2->oppositeHalfEdge = E2->oppositeHalfEdge;
				NE2->oppositeHalfEdge->oppositeHalfEdge = NE2;
		
				E2->oppositeHalfEdge = E2->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge;
				E2->oppositeHalfEdge->oppositeHalfEdge = E2;    
		
				//Add to edgepoint average for both sides
				Vertex* fp2 = &vertices[NE2->oppositeHalfEdge->nextHalfEdge->nextHalfEdge->vertex];
				NV2->pos = (originalVertices[iOV2].pos + originalVertices[iOV3].pos + fp->pos + fp2->pos) * 0.25f;
		
			}
			if(E3->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge == nullptr)
			{
				NE3->oppositeHalfEdge = E3->oppositeHalfEdge;
				NE3->oppositeHalfEdge->oppositeHalfEdge = NE3;
		
				E3->oppositeHalfEdge = E3->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge;
				E3->oppositeHalfEdge->oppositeHalfEdge = E3;    
		
				//Add to edgepoint average for both sides
				Vertex* fp2 = &vertices[NE3->oppositeHalfEdge->nextHalfEdge->nextHalfEdge->vertex];
				NV3->pos = (originalVertices[iOV3].pos + originalVertices[iOV4].pos + fp->pos + fp2->pos) * 0.25f;
		
			}
			if(E4->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge == nullptr)
			{
				NE4->oppositeHalfEdge = E4->oppositeHalfEdge;
				NE4->oppositeHalfEdge->oppositeHalfEdge = NE4;
		
				E4->oppositeHalfEdge = E4->oppositeHalfEdge->nextHalfEdge->oppositeHalfEdge->nextHalfEdge;
				E4->oppositeHalfEdge->oppositeHalfEdge = E4;
		
				//Add to edgepoint average for both sides
				Vertex* fp2 = &vertices[NE4->oppositeHalfEdge->nextHalfEdge->nextHalfEdge->vertex];
				NV4->pos = (originalVertices[iOV4].pos + originalVertices[iOV1].pos + fp->pos + fp2->pos) * 0.25f;
		
			}
	      
		
			//// OV1 /////
		
			// Valence will be 3 first time, but 4 each other time!
			int n = (iOV1 < 8 ? 3 : 4);
		
			int nn = n*n;

			//Q is facepoint / valence squared
			Math::Vector3 Q = fp->pos / nn;
		
			//-------
			//Get edges original midpoints and calculate average
			Math::Vector3 R = (((originalVertices[iOV1].pos + originalVertices[iOV2].pos)) / nn);
		
			Math::Vector3 S = (originalVertices[iOV1].pos * (n - 3)) / nn;
		
			OV1->pos = OV1->pos + (Q + R + S);
		
		
		
		
			//// OV2 /////
		
			// Valence will be 3 first time, but 4 each other time!
			n = (iOV2 < 8 ? 3 : 4);
			
			nn = n*n;

			//Q is facepoint / valence
			Q = fp->pos / nn;
		
			//-------
			//Get edges original midpoints and calculate average
			R = (((originalVertices[iOV2].pos + originalVertices[iOV3].pos)) / nn);
		
			S = (originalVertices[iOV2].pos * (n-3)) / (nn);
		
			OV2->pos = OV2->pos + (Q + R + S);
		
		
		
			//// OV3 /////
		
			// Valence will be 3 first time, but 4 each other time!
			n = (iOV3 < 8 ? 3 : 4);
		
			nn = n*n;

			//Q is facepoint / valence
			Q = fp->pos / nn;
		
			//-------
			//Get edges original midpoints and calculate average
			R = (((originalVertices[iOV3].pos + originalVertices[iOV4].pos)) / nn);
		
			S = (originalVertices[iOV3].pos * (n-3)) / (nn);
		
			OV3->pos = OV3->pos + (Q + R + S);
		
		
			//// OV4 /////
		
			// Valence will be 3 first time, but 4 each other time!
			n = (iOV4 < 8 ? 3 : 4);
			nn = n*n;

			//Q is facepoint / valence
			Q = fp->pos / nn;
		
			//-------
			//Get edges original midpoints and calculate average
			R = (((originalVertices[iOV4].pos + originalVertices[iOV1].pos)) / nn);
		
			S = (originalVertices[iOV4].pos * (n-3)) / (nn);
		
			OV4->pos = OV4->pos + (Q + R + S);
		}
	      
	    //struct rusage r_usage;
	    //getrusage(RUSAGE_SELF,&r_usage);
	    //printf("Memory usage: %ld bytes\n",r_usage.ru_maxrss);
	      
	    time = glfwGetTime() - time;
	    printf("subdiv %i : Time : %f s\n", x + 1, time);
	}
	    
}

	std::vector < MeshResource::Vertex >& MeshResource::getMesh() 
	{
		return this->mesh;
	}
	std::vector < GLuint >& MeshResource::getIndices() 
	{
		return this->indices;
	}

	void MeshResource::setupMesh()
	{
		glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object  
		glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it  

		glGenBuffers(1, this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), &mesh[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8 + sizeof(uintptr_t), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8 + sizeof(uintptr_t), (GLvoid*)(sizeof(float32) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8 + sizeof(uintptr_t), (GLvoid*)(sizeof(float32) * 5));

		glGenBuffers(1, this->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

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
		mesh.push_back(vert1);
		mesh.push_back(vert2);
		mesh.push_back(vert3);
		mesh.push_back(vert4);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object  
		glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it  

		glGenBuffers(1, this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), &mesh[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8 + sizeof(uintptr_t), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8 + sizeof(uintptr_t), (GLvoid*)(sizeof(float32) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8 + sizeof(uintptr_t), (GLvoid*)(sizeof(float32) * 5));

		glGenBuffers(1, this->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshResource::createCube()
	{
		/*
		MeshResource::vertex vert0 = { { 0.0f, 100.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert1 = { { 100.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert2 = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert3 = { { 100.0f, 100.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert4 = { { 0.0f, 100.0f, -1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert5 = { { 100.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert6 = { { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::vertex vert7 = { { 100.0f, 100.0f, -1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		
		mesh.push_back(vert0);
		mesh.push_back(vert1);
		mesh.push_back(vert2);
		mesh.push_back(vert3);
		mesh.push_back(vert4);
		mesh.push_back(vert5);
		mesh.push_back(vert6);
		mesh.push_back(vert7);

		indices.insert(indices.end(), {
			0, 3, 2,
			2, 3, 1,

			0, 7, 4,
			3, 7, 0,

			6, 1, 2,
			1, 5, 6,

			6, 0, 4,
			0, 2, 6,

			1, 7, 3,
			7, 1, 5,

			6, 7, 4,
			5, 7, 6
		});
		*/

		MeshResource::Vertex vert1 = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::Vertex vert2 = { { -0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::Vertex vert3 = { { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
		MeshResource::Vertex vert4 = { { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		mesh.push_back(vert1);
		mesh.push_back(vert2);
		mesh.push_back(vert3);
		mesh.push_back(vert4);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
	}

	void MeshResource::draw()
	{

		glBindVertexArray(vao[0]); // Bind our Vertex Array Object 

		// WITHOUT INDEXBUFFER

		/*
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
		glDrawArrays(GL_TRIANGLES, 0, mesh.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		*/

		// WITH INDEXBUFFERS
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0); // Unbind our Vertex Array Object  
		


	}
	
}