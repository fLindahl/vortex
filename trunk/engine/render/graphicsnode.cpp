#include "config.h"
#include "graphicsnode.h"

namespace Render
{
	GraphicsNode::GraphicsNode()
	{
	}

	GraphicsNode::~GraphicsNode()
	{

	}

	shared_ptr<MeshResource> GraphicsNode::getMesh()
	{
		return this->mesh;
	}

	void GraphicsNode::setMesh(shared_ptr<MeshResource> inMesh)
	{
		this->mesh = inMesh;
	}

	shared_ptr<TextureResource> GraphicsNode::getTexture()
	{
		return this->texture;
	}

	void GraphicsNode::setTexture(shared_ptr<TextureResource> inTexture)
	{
		this->texture = inTexture;
	}

	shared_ptr<ShaderObject> GraphicsNode::getShader()
	{
		return this->shader;
	}

	void GraphicsNode::setShader(shared_ptr<ShaderObject> inShader)
	{
		this->shader = inShader;
	}

	Math::Matrix4 GraphicsNode::getMatrix(MatrixType type)
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

	void GraphicsNode::setMatrix(MatrixType type, const Math::Matrix4 &mat)
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

	void GraphicsNode::draw()
	{

		this->shader->applyProgram();

		this->texture->bindTexture(0);

		this->shader->setUniMatrix4fv(this->modelMat, "modelMatrix");
		this->shader->setUniMatrix4fv(this->viewMat, "viewMatrix");
		this->shader->setUniMatrix4fv(this->projMat, "projectionMatrix");

		this->mesh->draw();

	}

}
