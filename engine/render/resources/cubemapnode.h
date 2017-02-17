#pragma once
#include "foundation/math/point.h"
#include "GL/glew.h"
#include <memory>
#include "shaderobject.h"
#include "render/server/renderdevice.h"
#include "geometryproxy.h"

namespace Render
{

class CubeMapNode : public std::enable_shared_from_this<CubeMapNode>
{
public:
	enum CubemapShape
	{
		BOX,
		SPHERE
	};

	CubeMapNode();
	~CubeMapNode();

	void GenerateCubeMap();
	GLuint GetCubeMap();
	
	void SetPosition(Math::point pos);
	const Math::point& GetPosition() const;

	void Activate();
	void Deactivate();

	void DeleteCubeMap();

	bool IsLoaded();

	void CalculateInfluenceWeights(const Math::point& pos);

	Math::vector& InnerScale() { return this->innerScale; }
	Math::vector& OuterScale() { return this->outerScale; }

	void SetShape(CubemapShape s) { this->shape = s; }
	CubemapShape GetShape() const { return this->shape; }

	float GetBlendFactor() { return this->blendFactor; }

	void SetGeometryProxy(std::shared_ptr<Render::GeometryProxy> proxy) { this->proxy = proxy; }
	std::shared_ptr<Render::GeometryProxy> GetGeometryProxy() { return this->proxy; }

private:
	friend class LightServer;

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
	bool isActive;

	//Used for approximating level geometry to correct reflection vector's intersection with the cubemap (parallax correction)
	std::shared_ptr<Render::GeometryProxy> proxy;

	//Cubemaps worldspace position
	Math::point position;

	// cubemaps influence shape
	// box will use entire scale vectors, sphere will always be spherical (x as radius)
	CubemapShape shape;
	// if camera is within innerbounds, 100% of this cubemap will be used.
	Math::vector innerScale;
	// cubemaps will be blended depending on the cameras position relative to inner and outer range for k cubemaps.
	Math::vector outerScale;

	Render::Resolution resolution;
	GLuint mipLevels;

	///The cubemap
	GLuint cubeSampler;

	// normalized distance function which is 0 at center and 1 at boundary
	float NDF;
	// how much to blend this cubemap into the final color
	float blendFactor;

#ifdef _LEVELEDITOR
	//Lists all textures in the cubemap so that we can inspect them within applications
	GLuint textures[6];
#endif
};
}