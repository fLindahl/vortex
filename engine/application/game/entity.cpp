#include "config.h"
#include "entity.h"
#include "foundation/messaging/messagehandler.h"

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

void Entity::HandleMsg(Msg msg)
{

}

void Entity::SendMsg(Msg msg)
{
	MsgHandler::getInstance()->RecvMsg(msg);
}

void Entity::SendMsg(int recipientID, MsgType message, float delay)
{
	Msg newMsg;
	newMsg.recipientID = recipientID;
	newMsg.senderID = this->getID();
	newMsg.message = message;
	newMsg.delay = delay;

	MsgHandler::getInstance()->RecvMsg(newMsg);
}

Math::Vector4& Entity::getPos()
{
	return pos;
}

void Entity::setPos(Math::Vector4 nPos)
{
	this->pos = nPos;
}

}

