#include "config.h"
#include "graphicsproperty.h"
#include "render/resources/meshresource.h"
#include "render/resources/modelinstance.h"

namespace Render
{
	GraphicsProperty::GraphicsProperty()
	{
	}

	GraphicsProperty::~GraphicsProperty()
	{
		if (this->modelInstance != nullptr)
		{
			//HACK: Implement this
			//this->modelInstance->removeGraphicsProperty(this);
		}
	}

	shared_ptr<ModelInstance> GraphicsProperty::getModelInstance() const
	{
		return this->modelInstance;
	}

	void GraphicsProperty::setModelInstance(shared_ptr<ModelInstance> inModelInstance)
	{
		//HACK: Implement this
		//if (this->modelInstance.get() != nullptr)
		//{
		//	this->modelInstance->removeGraphicsProperty(this);
		//}

		this->modelInstance = inModelInstance;
		this->modelInstance->addGraphicsProperty(this);

	}

	Math::Matrix4 GraphicsProperty::getModelMatrix() const
	{
		return this->modelMat;
	}

	void GraphicsProperty::setModelMatrix(const Math::Matrix4 &mat)
	{
		this->modelMat = mat;
	}
}
