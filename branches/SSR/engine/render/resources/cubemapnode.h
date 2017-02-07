#pragma once
#include "foundation/math/point.h"
#include "GL/glew.h"
#include <memory>
#include "shaderobject.h"
#include "render/server/renderdevice.h"

namespace Render
{

class CubeMapNode
{
public:
	CubeMapNode();
	~CubeMapNode();

	void GenerateCubeMap();
	GLuint GetCubeMap();
	
	void SetPosition(Math::point pos);

	void DeleteCubeMap();

	bool IsLoaded();

private:
	enum CubeFace
	{
		RIGHT = 0,
		LEFT = 1,
		TOP = 2,
		BOTTOM = 3,
		BACK = 4,
		FRONT = 5
	};

	void RenderTexture(const GLuint& framebuffer, CubeFace face, Graphics::Camera& camera);

	bool isLoaded;

	Math::point position;

	Render::Resolution resolution;
	GLuint mipLevels;

	///The cubemap
	GLuint cubeSampler;

	//Lists all textures in the cubemap
	GLuint textures[6];
};
}