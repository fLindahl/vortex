#include "config.h"
#include "entity.h"

namespace Game
{

Entity::Entity()
{
	//Create graphics object and add it to graphicsserver.
	//this->graphics = std::make_shared<Render::GraphicsProperty>(this);
	
	//GraphicsServer::getInstance()->addGraphicsProperty(this->graphics);
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

void Entity::HandleMsg(std::shared_ptr<BaseGameFeature::Msg> msg)
{

}

void Entity::SendMsg(std::shared_ptr<BaseGameFeature::Msg> msg)
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
void Entity::SetTransform(const Math::mat4 &nTransform)
{
    this->transform = nTransform;
}

Math::mat4 Entity::GetTransform()
{
    return this->transform;
}

void Entity::AddProperty(std::shared_ptr<Game::BaseProperty> p)
{
    this->properties.Append(p);
	//TODO: This should work
    //p->owner = std::shared_from_this();
}

void Entity::Activate()
{

}

void Entity::Deactivate()
{

}

}

