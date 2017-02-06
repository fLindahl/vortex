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

	void GenerateCubeMap(const Math::point& wPos);
	GLuint GetCubeMap();

private:
	bool IsLoaded;

	Render::Resolution resolution;
	GLuint mipLevels;

	///The cubemap
	GLuint cubeSampler;

	///Top texture
	GLuint top;
	///Bottom texture
	GLuint bottom;
	///Left texture
	GLuint left;
	///Right texture
	GLuint right;
	///Back texture
	GLuint back;
	///Front texture
	GLuint front;
};
}