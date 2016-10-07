#pragma once
#include <memory>
#include "foundation/math/matrix4.h"
#include "foundation/math/matrix3.h"
#include "foundation/math/vector4.h"

namespace Render
{

class MeshResource;
class TextureResource;
class ShaderObject;

enum ShaderType
{
	VERTEX,
	FRAGMENT
};

enum MatrixType
{
	MODEL,
	PROJECTION,
	VIEW
};

class GraphicsProperty
{
public:
	GraphicsProperty();
	~GraphicsProperty();

	shared_ptr<MeshResource> getMesh();
	void setMesh(shared_ptr<MeshResource> inMesh);

	shared_ptr<TextureResource> getTexture();
	void setTexture(shared_ptr<TextureResource> inTexture);

	shared_ptr<ShaderObject> getShader();
	void setShader(shared_ptr<ShaderObject> inShader);

	Math::Matrix4 getMatrix(MatrixType type);
	void setMatrix(MatrixType type, const Math::Matrix4 &mat);

	void draw();

private:
	shared_ptr<MeshResource> mesh;
	shared_ptr<TextureResource> texture;
	shared_ptr<ShaderObject> shader;
	Math::Matrix4 modelMat;
	Math::Matrix4 viewMat;
	Math::Matrix4 projMat;
};

}