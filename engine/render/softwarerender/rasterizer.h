/**
Rasterizer
For Software Rendering.
**/

#pragma once
#include "config.h"
#include "render/resources/meshresource.h"
#include "foundation/util/array.h"
#include <memory>
#include "foundation/math/matrix3.h"
#include "foundation/math/matrix4.h"
#include "foundation/math/vector4.h"
#include <functional>
#include "stb_image.h"

namespace Render
{
enum Octant
{
	Octant1,
	Octant2,
	Octant3,
	Octant4
};

class ScanLine
{
public:
	//Color color;
	int Y;
	int X1, X2;

	ScanLine(int x1, int x2, int y);
};

class Edge
{
public:
	Edge(Math::Vector4 v1, Math::Vector4 v2);

	Octant type;
	int xb;
	int yb;
	int xe;
	int ye;
	int zb;
	int ze;

	int dx, dy, incE, incNE, d, x, y;

};

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	struct Color
	{
		unsigned char r = 1;
		unsigned char g = 1;
		unsigned char b = 1;
	};

	struct Texture
	{
		int w, h, n; //Width, Height, components per pixel (ex. RGB = 3, RGBA = 4)
		unsigned char *image;
	};

	static const int screenwidth = 800;
	static const int screenheight = 600;

	void clear();
	
	void setup(Math::Matrix4 pMat, Math::Matrix4 tMat); // pMat = Perspective Matrix, tMat = transform Matrix
	Util::Array<Color> draw();
	
	void setVertexShader(const std::function<Math::Vector3(const Math::Matrix4& pMat, const Math::Matrix4& tMat, const MeshResource::Vertex& vert, Math::Vector4& outVert)>& func);
	std::function<Math::Vector3(const Math::Matrix4& pMat, const Math::Matrix4& tMat, const MeshResource::Vertex& vert, Math::Vector4& outVert)> vertexShader;

	void setFragmentShader(const std::function<Color(const Math::Vector2& uv, const Texture& texture)>& func);
	std::function<Color(const Math::Vector2& uv, const Texture& texture)> fragmentShader;

	//void texturize();

	void setTexture(const char* filename);

	void RenderTriangle();
	void scanline(const ScanLine &scanline);
	void increment(Edge &edge);
	void addPixel(const int& x, const int& y);
	void barycentric(const Math::Vector2& p, const Math::Vector2& a, const Math::Vector2& b, const Math::Vector2& c, float &u, float &v, float &w);

	void addPixel2(int x, int y);
	shared_ptr<MeshResource> getMesh();
	void setMesh(shared_ptr<MeshResource> inMesh);

private:
	Texture texture;

	//Currently in-use vertices
	MeshResource::Vertex v1, v2, v3;
	//Currently in-use W Components
	float w1, w2, w3;
	//Currently in-use diffusecolors
	Math::Vector3 diffuse1, diffuse2, diffuse3;

	std::shared_ptr<MeshResource> mesh;
	Util::Array<Color> pixels;
	Util::Array<GLfloat> depthBuffer;
};
}