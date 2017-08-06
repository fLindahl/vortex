#include "config.h"
#include "entity.h"
#include "application/game/baseproperty.h"

namespace Game
{
__ImplementClass(Game::Entity, 'NTTY', Core::RefCounted);

Entity::Entity()
{
	//Create graphics object and add it to graphicsserver.
	//this->graphics = std::make_shared<Render::GraphicsProperty>(this);
	
	//GraphicsServer::getInstance()->addGraphicsProperty(this->graphics);

	this->ID = BaseGameFeature::EntityManager::Instance()->GetNewEntityID();
	this->baseBBox = NULL;
}

Entity::~Entity()
{
	//GraphicsServer::getInstance()->removeGraphicsProperty(this->graphics);
}
	
void Entity::FixedUpdate()
{
	for (int i = 0; i < this->properties.Size(); i++)
	{
		this->properties[i]->FixedUpdate();
	}
}

void Entity::Update()
{
	for (int i = 0; i < this->properties.Size(); i++)
	{
		this->properties[i]->Update();
	}
}

void Entity::HandleMessage(const Ptr<Messaging::Message>& msg)
{
	if (msg->GetType() == Msg::SetTransform::Type)
	{
		const Ptr<Msg::SetTransform>& m = msg.downcast<Msg::SetTransform>();
		this->transform = m->Get();
	}

	//Have all the properties handle the same messge
	for (int i = 0; i < this->properties.Size(); i++)
	{
		auto prop = this->properties[i];
		prop->HandleMessage(msg);
	}
	
}

void Entity::SetTransform(const Math::mat4& nTransform)
{
    this->transform = nTransform;
	this->UpdateBBox();

	Ptr<Msg::SetTransform> setMsg = Msg::SetTransform::Create();
	setMsg->Set(this->transform);

	this->HandleMessage(setMsg.downcast<Messaging::Message>());
}

Math::mat4 Entity::GetTransform()
{
    return this->transform;
}

void Entity::AddProperty(const Ptr<Game::BaseProperty>& p)
{
    this->properties.Append(p);
    p->owner = this;
}

void Entity::RemoveProperty(const Ptr<Game::BaseProperty>& p)
{
	index_t index = this->properties.FindIndex(p);
	this->properties.RemoveIndexSwap(index);
}

void Entity::Activate()
{
	BaseGameFeature::EntityManager::Instance()->RegisterEntity(this);
	
	for (auto& property : this->properties)
	{
		property->Activate();
	}

	this->active = true;
}

void Entity::Deactivate()
{
	BaseGameFeature::EntityManager::Instance()->UnregisterEntity(this->ID);
	this->active = false;
}

size_t Entity::GetNumProperties()
{
	return this->properties.Size();
}

Ptr<BaseProperty>& Entity::Property(uint index)
{
	_assert(index < this->properties.Size());
	
	return this->properties[index];
}

const bool& Entity::IsActive() const
{
	return this->active;
}

void Entity::SetBaseBBox(const Math::bbox* box)
{
	this->baseBBox = box;
	this->UpdateBBox();
}

void Entity::UpdateBBox()
{
	if (this->baseBBox == NULL)
		return;

	this->bbox = *this->baseBBox;
	this->bbox.transform(this->transform);
}

}

