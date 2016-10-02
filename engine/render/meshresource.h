#pragma once
#include <cstddef>
#include "GL/glew.h"
#include <string>
#include <fstream>
#include <vector>
#include "foundation/math/vector4.h"
#include "foundation/math/vector3.h"
#include "foundation/math/vector2.h"
#include <map>
#include <utility>
#include "foundation/util/array.h"

namespace Render
{

class MeshResource{
public:
	MeshResource();
	~MeshResource();

	struct Face;
	struct HalfEdge;
	
	struct Vertex
	{
		bool operator==(const Vertex& rhs) const
		{
			return ( pos == rhs.pos && uv == rhs.uv && normal == rhs.normal);
		}
		
		Math::Vector3 pos;
		Math::Vector2 uv;
		Math::Vector3 normal;
		
		HalfEdge* halfedge;
	};

	struct HalfEdge
	{
	      bool operator==(HalfEdge* rhs) 
	      {
		return ((uintptr_t)this == (uintptr_t)rhs);
	      }
	  
	      bool operator!=(HalfEdge* rhs) 
	      {
		return ( (uintptr_t)this != (uintptr_t)rhs );
	      }
	  
	      HalfEdge* oppositeHalfEdge;
	      HalfEdge* nextHalfEdge;
	      size_t vertex;
	      Face* face;
	};

	struct Face
	{
	  HalfEdge* edge;
	};

	void Subdivide(const int& amount);
	
	bool loadMeshFromFile(const char* filename);
	
	void setupMesh();
	
	bool generateHalfEdgeMeshFromFile(const char* filename);
	bool loadMeshFromHalfEdgeMesh();
	
	void createQuad();
	void createCube();
	void draw();
	std::vector < MeshResource::Vertex >& getMesh();
	std::vector < GLuint >& getIndices();

private:
	//void stringToVector(std::string &str, std::vector<std::string> &v);
	
	GLuint vao[1];
	GLuint ib[1];
	GLuint vbo[1];
	
	//std::vector<Vertex*> vertices;

	Array<Vertex> vertices;
	
	Array<HalfEdge> edges;

	Array<Face> faces;
	
	//std::vector<Face*> faces;

	std::vector < MeshResource::Vertex > mesh;
	std::vector < GLuint > indices;
};

}