#include "config.h"
#include "graphicsproperty.h"
#include "render/resources/meshresource.h"
#include "render/resources/lightnode.h"
#include "render/resources/textureresource.h"


namespace Render
{
	GraphicsProperty::GraphicsProperty()
	{
	}

	GraphicsProperty::~GraphicsProperty()
	{

	}

	shared_ptr<MeshResource> GraphicsProperty::getMesh()
	{
		return this->mesh;
	}

	void GraphicsProperty::setMesh(shared_ptr<MeshResource> inMesh)
	{
		this->mesh = inMesh;
	}

	shared_ptr<TextureResource> GraphicsProperty::getTexture()
	{
		return this->texture;
	}

	void GraphicsProperty::setTexture(shared_ptr<TextureResource> inTexture)
	{
		this->texture = inTexture;
	}

	shared_ptr<ShaderObject> GraphicsProperty::getShader()
	{
		return this->shader;
	}

	void GraphicsProperty::setShader(shared_ptr<ShaderObject> inShader)
	{
		this->shader = inShader;
	}

	Math::Matrix4 GraphicsProperty::getMatrix(MatrixType type)
	{
		if (type == MODEL)
		{
			return this->modelMat;
		}
		else if (type == PROJECTION)
		{
			return this->projMat;
		}
		else if (type == VIEW)
		{
			return this->viewMat;
		}
	}

	void GraphicsProperty::setMatrix(MatrixType type, const Math::Matrix4 &mat)
	{
		if (type == MODEL)
		{
			this->modelMat = mat;
		}
		else if (type == PROJECTION)
		{
			this->projMat = mat;
		}
		else if (type == VIEW)
		{
			this->viewMat = mat;
		}
	}

	void GraphicsProperty::draw()
	{

		this->shader->applyProgram();

		this->texture->bindTexture(0);

		this->shader->setUniMatrix4fv(this->modelMat, "modelMatrix");
		this->shader->setUniMatrix4fv(this->viewMat, "viewMatrix");
		this->shader->setUniMatrix4fv(this->projMat, "projectionMatrix");

		this->mesh->draw();

	}

}
