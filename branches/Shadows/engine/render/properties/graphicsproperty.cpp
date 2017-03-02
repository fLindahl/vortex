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
		this->modelInstance->RemoveGraphicsProperty(this);
	}
}

std::shared_ptr<Render::ModelInstance> GraphicsProperty::getModelInstance() const
{
	return this->modelInstance;
}

void GraphicsProperty::setModelInstance(std::shared_ptr<Render::ModelInstance> inModelInstance)
{
	if (this->modelInstance != nullptr)
	{
		this->modelInstance->RemoveGraphicsProperty(this);
	}

	this->modelInstance = inModelInstance;
}

const Math::mat4& GraphicsProperty::getModelMatrix() const
{
	return this->modelMat;
}

const Math::mat4& GraphicsProperty::getInvModelMatrix() const
{
	return this->modelMat;
}

void GraphicsProperty::setModelMatrix(const Math::mat4 &mat)
{
	this->modelMat = mat;
	this->invMat = Math::mat4::inverse(this->modelMat);
	this->bbox = this->modelInstance->GetMesh()->getBaseBBox();
	this->bbox.transform(this->modelMat);
}

void GraphicsProperty::Update()
{
	this->Game::BaseProperty::Update();
}

void GraphicsProperty::Activate()
{
	if (!this->active)
	{
		if (this->modelInstance != nullptr)
		{
			this->modelInstance->AddGraphicsProperty(this);
			this->bbox = this->modelInstance->GetMesh()->getBaseBBox();
			this->bbox.transform(this->getModelMatrix());
		}
		else
		{
			printf("ERROR: GraphicsProperty::Activate() >> No ModelInstance found!\n");
			assert(false);
		}

		BaseProperty::Activate();
	}
}

void GraphicsProperty::Deactivate()
{
	if (this->active)
	{
		this->modelInstance->RemoveGraphicsProperty(this);
		BaseProperty::Deactivate();
	}
}

}