#include "config.h"
#include "rasterizer.h"
#include <algorithm>
//#define WIREFRAME

namespace Render
{

ScanLine::ScanLine(int x1, int x2, int y)
{
	Y = y;
	if (x1 < x2)
	{
		X1 = x1;
		X2 = x2;
	}
	else
	{
		X1 = x2;
		X2 = x1;
	}
}

Edge::Edge(Math::Vector4 v1, Math::Vector4 v2)
{
	if (v1[1] < v2[1])
	{
		xb = floor(v1[0]);
		yb = floor(v1[1]);
		zb = floor(v1[2]);
		xe = floor(v2[0]);
		ye = floor(v2[1]);
		ze = floor(v2[2]);
	}
	else
	{
		xb = floor(v2[0]);
		yb = floor(v2[1]);
		zb = floor(v2[2]);
		xe = floor(v1[0]);
		ye = floor(v1[1]);
		ze = floor(v1[2]);
	}

	dx = (xe - xb);
	dy = (ye - yb);

	if (dx >= 0)
	{
		if (dy > dx)
		{
			this->type = Octant2;
			swap(dx, dy);
		}
		else
			this->type = Octant1;
	}
	else
	{
		if (dy > -dx)
		{
			this->type = Octant3;
			swap(dx, dy);
		}
		else
			this->type = Octant4;
	}

	dx = abs(dx);
	dy = abs(dy);

	d = 2 * dy - dx;
	incE = 2 * dy;
	incNE = 2 * (dy - dx);

	x = xb;
	y = yb;
}

Rasterizer::Rasterizer()
{
	clear();
}

Rasterizer::~Rasterizer()
{
	stbi_image_free(texture.image);
}

void Rasterizer::clear()
{
	Color clr;
	clr.r = 185;
	clr.g = 185;
	clr.b = 185;

	this->pixels.Fill(0, screenwidth*screenheight, clr);

	this->depthBuffer.Fill(0, screenwidth*screenheight, 200000000.0f); //WORKAROUND: Should be 0
}

void Rasterizer::setup(Math::Matrix4 pMat, Math::Matrix4 tMat)
{
	/*
	const Util::Array<MeshResource::Vertex>& meshData = mesh->getMesh();
	const Util::Array< GLuint >& meshIndices = mesh->getIndices();

	Math::Vector4 vertPos1, vertPos2, vertPos3;

	for (int i = 0; i < meshIndices.Size(); i += 3)
	{
		diffuse1 = vertexShader(pMat, tMat, meshData[meshIndices[i]], vertPos1);
		diffuse2 = vertexShader(pMat, tMat, meshData[meshIndices[i + 1]], vertPos2);
		diffuse3 = vertexShader(pMat, tMat, meshData[meshIndices[i + 2]], vertPos3);

		w1 = vertPos1[3];
		w2 = vertPos2[3];
		w3 = vertPos3[3];

		v1 = {{ ((vertPos1[0] / w1 + 1) / 2) * screenwidth, ((vertPos1[1] / w1 + 1) / 2) * screenheight, vertPos1[2] / w1 }, meshData[meshIndices[i]].uv, meshData[meshIndices[i]].normal };
		v2 = {{ ((vertPos2[0] / w2 + 1) / 2) * screenwidth, ((vertPos2[1] / w2 + 1) / 2) * screenheight, vertPos2[2] / w2 }, meshData[meshIndices[i + 1]].uv, meshData[meshIndices[i + 1]].normal };
		v3 = {{ ((vertPos3[0] / w3 + 1) / 2) * screenwidth, ((vertPos3[1] / w3 + 1) / 2) * screenheight, vertPos3[2] / w3 }, meshData[meshIndices[i + 2]].uv, meshData[meshIndices[i + 2]].normal };

		RenderTriangle();
	}
	*/
}

Util::Array<Rasterizer::Color> Rasterizer::draw()
{
	return this->pixels;
}

void Rasterizer::setVertexShader(const std::function<Math::Vector3(const Math::Matrix4& pMat, const Math::Matrix4& tMat, const MeshResource::Vertex& vertex, Math::Vector4& outVert)>& func)
{
	this->vertexShader = func;
}

void Rasterizer::setFragmentShader(const std::function<Color(const Math::Vector2& uv, const Texture& texture)>& func)
{
	this->fragmentShader = func;
}

/*
void Rasterizer::texturize()
{
	int u = abs((int)(uv[0] * texture.w) % texture.w);
	int v = abs((int)(uv[1] * texture.h) % texture.h);

	int pos = (u + v * texture.w) * 4;

	Color color;

	color.b = texture.image[pos];
	color.g = texture.image[pos + 1];
	color.r = texture.image[pos + 2];
	//color.a = internalBuffer[pos + 3];
}
*/

void Rasterizer::setTexture(const char* filename)
{
	//Width, Height, components per pixel (ex. RGB = 3, RGBA = 4)
	texture.image = stbi_load(filename, &texture.w, &texture.h, &texture.n, 0);
}

void Rasterizer::RenderTriangle()
{
	Edge edges[3] = {
		Edge(v1.pos , v2.pos),
		Edge(v1.pos, v3.pos),
		Edge(v3.pos, v2.pos)
	};

	int maxlength = 0;
	int longEdge = 0;

	// find edge with the greatest length in the y axis
	for (int i = 0; i < 3; i++) {
		int length = edges[i].ye - edges[i].yb;
		if (length > maxlength) {
			maxlength = length;
			longEdge = i;
		}
	}

	int shortEdge1, shortEdge2;

	if (edges[(longEdge + 1) % 3].yb > edges[(longEdge + 2) % 3].yb)
	{
		shortEdge2 = (longEdge + 1) % 3;
		shortEdge1 = (longEdge + 2) % 3;
	}
	else
	{
		shortEdge1 = (longEdge + 1) % 3;
		shortEdge2 = (longEdge + 2) % 3;
	}

	//addPixel(edges[longEdge].x, edges[longEdge].y, v1, v2, v3, w1, w2, w3, diffuse1, diffuse2, diffuse3);

	while (edges[longEdge].y < edges[longEdge].ye)
	{
		int oldy = edges[longEdge].y;
		while (edges[longEdge].y == oldy)
		{
			increment(edges[longEdge]);
			#ifdef WIREFRAME
			addPixel2(edges[longEdge].x, edges[longEdge].y);
			#endif

		}
		
		if (edges[longEdge].y <= edges[shortEdge1].ye)
		{
			while (edges[longEdge].y > edges[shortEdge1].y)
			{
				increment(edges[shortEdge1]);
				#ifdef WIREFRAME
				addPixel2(edges[shortEdge1].x, edges[shortEdge1].y);
				#endif
			}
			scanline(ScanLine(edges[shortEdge1].x, edges[longEdge].x, edges[longEdge].y));
		}
		else
		{
			while (edges[longEdge].y > edges[shortEdge2].y)
			{
				increment(edges[shortEdge2]);
				#ifdef WIREFRAME
				addPixel2(edges[shortEdge2].x, edges[shortEdge2].y);
				#endif
				
			}
			scanline(ScanLine(edges[shortEdge2].x, edges[longEdge].x, edges[longEdge].y));
		}

	}

}

void Rasterizer::scanline(const ScanLine &scanline)
{
	//int xdiff = scanline.X2 - scanline.X1;
	//if (xdiff == 0)
	//	return;

	// draw each pixel in the span
	for (int x = scanline.X1; x < scanline.X2; x++) 
	{
		addPixel(x, scanline.Y);
	}
}

void Rasterizer::increment(Edge &edge)
{
	if (edge.type == Octant1)
	{
		edge.x++;
		if (edge.d < 0)     /* choose E */
			edge.d += edge.incE;
		else 
		{
			edge.d += edge.incNE; /* choose NE */
			edge.y += 1;
		}
	}
	else if (edge.type == Octant2)
	{
		edge.y++;
		if (edge.d < 0)     /* choose E */
			edge.d += edge.incE;
		else
		{
			edge.d += edge.incNE; /* choose NE */
			edge.x += 1;
		}
	}
	else if (edge.type == Octant3)
	{
		edge.y++;
		if (edge.d < 0)     /* choose E */
			edge.d += edge.incE;
		else
		{
			edge.d += edge.incNE; /* choose NE */
			edge.x -= 1;
		}
	}
	else if (edge.type == Octant4)
	{
		edge.x--;
		if (edge.d < 0)     /* choose E */
			edge.d += edge.incE;
		else {
			edge.d += edge.incNE; /* choose NE */
			edge.y += 1;
		}
	}
}

void Rasterizer::addPixel(const int& x, const int& y)
{
	if (x >= screenwidth)
		return;
	if (x < 0)
		return;
	if ((y * screenwidth + x) < 0 )
		return;
	if ((y * screenwidth + x) >= screenwidth*screenheight)
		return;
	
	float u;
	float v;
	float w;

	barycentric(Math::Vector2(x, y), Math::Vector2(floor(v1.pos[0]), floor(v1.pos[1])), Math::Vector2(floor(v2.pos[0]), floor(v2.pos[1])), Math::Vector2(floor(v3.pos[0]), floor(v3.pos[1])), u, v, w);
	float corrW = (u / w1 + v / w2 + w / w3); // TODO: UVW FÅR INTE VARA NEGATIVA!


	float pixelZ = ((v1.pos[2] * u) + (v2.pos[2] * v) + (v3.pos[2] * w)) / corrW;
	int index = (y * this->screenwidth + x);

	if (depthBuffer[index] < pixelZ)
	{
		return; // Discard
	}

	depthBuffer[index] = pixelZ;
	

	Math::Vector2 pixelUV = ((v1.uv*u) / w1 + (v2.uv*v) / w2 + (v3.uv*w) / w3) / corrW;
	

	Color clr = fragmentShader({ pixelUV[0], 1 - pixelUV[1] }, this->texture);

	Math::Vector3 diffuseColor = ((diffuse1 * u) / w1 + (diffuse2 * v) / w2 + (diffuse3 * w) / w3) / corrW;
	
	float R = floor(clr.r);
	float G = floor(clr.g);
	float B = floor(clr.b);

	// Factor Diffusecolor
	clr.r = (int)(R * diffuseColor[0]);
	clr.g = (int)(G * diffuseColor[1]);
	clr.b = (int)(B * diffuseColor[2]);

	this->pixels[index] = clr;
}

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
void Rasterizer::barycentric(const Math::Vector2& p, const Math::Vector2& a, const Math::Vector2& b, const Math::Vector2& c, float &u, float &v, float &w)
{
	//TODO: Optimize. some of this can be done once per triangle
	Math::Vector2 v0 = b - a, v1 = c - a, v2 = p - a;
	int d00 = v0.dot(v0);
	int d01 = v0.dot(v1);
	int d11 = v1.dot(v1);
	int d20 = v2.dot(v0);
	int d21 = v2.dot(v1);
	float denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;

	//HACK: clamp. makes holes in the mesh. Doesn't make it prettier :(
 	v = std::max(0.0f, std::min(v, 1.0f));
 	w = std::max(0.0f, std::min(w, 1.0f));
 	u = std::max(0.0f, std::min(u, 1.0f));

}

void Rasterizer::addPixel2(int x, int y)
{
	if (x >= screenwidth)
		return;
	if (x < 0)
		return;
	if ((y * screenwidth + x) < 0)
		return;
	if ((y * screenwidth + x) >= screenwidth*screenheight)
		return;

	Color pixel;
	pixel.r = 255; //WORKAROUND: should use texture color later.
	pixel.g = 0;
	pixel.b = 0;

	this->pixels[y * screenwidth + x] = pixel; //to get the correct index in array
}

shared_ptr<MeshResource> Rasterizer::getMesh()
{
	return this->mesh;
}

void Rasterizer::setMesh(shared_ptr<MeshResource> inMesh)
{
	this->mesh = inMesh;
}

}