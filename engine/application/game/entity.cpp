#include "config.h"

/*
#include "entity.h"
#include "messages/MsgHandler.h"
#include "render/server/graphicsserver.h"

namespace Game
{

BaseEntity::BaseEntity()
{
	//Create graphics object and add it to graphicsserver.
	this->graphics = std::make_shared<Render::GraphicsProperty>(this);
	GraphicsServer::getInstance()->addGraphicsProperty(this->graphics);
}

BaseEntity::~BaseEntity()
{
	GraphicsServer::getInstance()->removeGraphicsProperty(this->graphics);
}
	
void BaseEntity::FixedUpdate()
{

}

void BaseEntity::Update()
{

}

void BaseEntity::HandleMsg(Msg msg)
{

}

void BaseEntity::SendMsg(Msg msg)
{
	MsgHandler::getInstance()->RecvMsg(msg);
}

void BaseEntity::SendMsg(int recipientID, MsgType message, float delay)
{
	Msg newMsg;
	newMsg.recipientID = recipientID;
	newMsg.senderID = this->getID();
	newMsg.message = message;
	newMsg.delay = delay;

	MsgHandler::getInstance()->RecvMsg(newMsg);
}

Math::Vector2& BaseEntity::getPos()
{
	return pos;
}

void BaseEntity::setPos(Math::Vector2 nPos)
{
	this->pos = nPos;
	graphics->updateModelMatrix();
}

}

 */