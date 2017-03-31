#include "config.h"
#include "entity.h"
#include "application\game\baseproperty.h"

namespace Game
{
__ImplementClass(Game::Entity, 'NTTY', Core::RefCounted);

Entity::Entity()
{
	//Create graphics object and add it to graphicsserver.
	//this->graphics = std::make_shared<Render::GraphicsProperty>(this);
	
	//GraphicsServer::getInstance()->addGraphicsProperty(this->graphics);

	this->ID = BaseGameFeature::EntityManager::Instance()->GetNewEntityID();
}

Entity::~Entity()
{
	//GraphicsServer::getInstance()->removeGraphicsProperty(this->graphics);
}
	
void Entity::FixedUpdate()
{

}

void Entity::Update()
{

}

void Entity::HandleMsg(Ptr<BaseGameFeature::Msg> msg)
{

}

void Entity::SendMsg(Ptr<BaseGameFeature::Msg> msg)
{
	//MsgHandler::getInstance()->RecvMsg(msg);
}

void Entity::SendMsg(const int& recipientID, const BaseGameFeature::MsgType& message, const float& delay)
{
	BaseGameFeature::Msg newMsg;
	newMsg.recipientID = recipientID;
	newMsg.senderID = this->getID();
	newMsg.message = message;
	newMsg.delay = delay;

	//MsgHandler::getInstance()->RecvMsg(newMsg);
}
void Entity::SetTransform(const Math::mat4& nTransform)
{
    this->transform = nTransform;
}

Math::mat4 Entity::GetTransform()
{
    return this->transform;
}

void Entity::AddProperty(Ptr<Game::BaseProperty> p)
{
    this->properties.Append(p);
    p->owner = this;
}

void Entity::Activate()
{
	BaseGameFeature::EntityManager::Instance()->RegisterEntity(this);
	this->active = true;
}

void Entity::Deactivate()
{
	BaseGameFeature::EntityManager::Instance()->UnregisterEntity(this->ID);
	this->active = false;
}
}

