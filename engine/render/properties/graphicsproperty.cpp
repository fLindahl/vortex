#include "config.h"
#include "graphicsproperty.h"
#include "render/resources/meshresource.h"
#include "render/resources/modelinstance.h"

namespace Render
{
__ImplementClass(Render::GraphicsProperty, 'GPPR', Game::BaseProperty);

GraphicsProperty::GraphicsProperty()
{
	castShadows = true;
}

GraphicsProperty::~GraphicsProperty()
{
	if (this->modelInstance != nullptr)
	{
		this->modelInstance->RemoveGraphicsProperty(this);
	}
}

Ptr<Render::ModelInstance> GraphicsProperty::getModelInstance() const
{
	return this->modelInstance;
}

void GraphicsProperty::setModelInstance(const Ptr<Render::ModelInstance>& inModelInstance)
{
	if (this->modelInstance != nullptr)
	{
		this->modelInstance->RemoveGraphicsProperty(this);
	}

	this->modelInstance = inModelInstance;

	//Make sure all other properties can react on change of mesh.
	Ptr<Msg::SetMesh> setMeshMsg = Msg::SetMesh::Create();
	setMeshMsg->Set(this->modelInstance->GetMesh());
	__SendMsg(this->owner, setMeshMsg);

	this->owner->SetBaseBBox(&this->modelInstance->GetMesh()->getBaseBBox());

	if (this->active)
	{
		this->modelInstance->AddGraphicsProperty(this);
	}
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
	this->owner->GetBBox() = this->modelInstance->GetMesh()->getBaseBBox();
	this->owner->GetBBox().transform(this->getModelMatrix());
}

bool GraphicsProperty::GetCastShadows() const
{
	return this->castShadows;
}

void GraphicsProperty::SetCastShadows(const bool & value)
{
	this->castShadows = value;
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
			this->owner->GetBBox() = this->modelInstance->GetMesh()->getBaseBBox();
			this->owner->GetBBox().transform(this->getModelMatrix());
		}
		else
		{
			_error("GraphicsProperty::Activate() >> No ModelInstance found!");
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

void GraphicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
	//Handle set transform message
	if (msg->GetType() == Msg::SetTransform::Type)
	{
		this->setModelMatrix(msg.cast<Msg::SetTransform>()->Get());
	}
}

void GraphicsProperty::Serialize(IO::Stream* stream)
{

}

}
