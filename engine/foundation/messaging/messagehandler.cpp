#include "config.h"
#include "messagehandler.h"
//#include "..\handlers\GameHandler.h"

void MsgHandler::RecvMsg(Msg msg)
{
	msgQueue.push(msg);
}

void MsgHandler::SendMsg(Msg msg)
{
	/*
	if (msg.recipientID == -1)  // Send to all entities
	{
		for (std::map<int, BaseEntity*>::iterator it = GameHandler::getInstance()->getEntityList().begin(); it != GameHandler::getInstance()->getEntityList().end(); ++it)
		{
			it->second->HandleMsg(msg);
		}
	}
	else
	{
		GameHandler::getInstance()->getEntityList().find(msg.recipientID)->second->HandleMsg(msg);
	}
	*/
}

void MsgHandler::Update()
{
	
	// Go through the entire msgQueue sending and deleting messages until iterator finds a msg that shouldn't be sent, then break the loop.
	float currTime = (float)clock() / CLOCKS_PER_SEC;

	while (true)
	{
		if (!msgQueue.empty())
		{
			if ((float)msgQueue.top().timeStamp / CLOCKS_PER_SEC + msgQueue.top().delay < currTime)
			{
				//std::cout << "MSGHANDLER:\tSending Message." << std::endl;
				Msg telegram = msgQueue.top();
				msgQueue.pop();
				this->SendMsg(telegram);
			}
			else
			{
				//std::cout << "MSGHANDLER:\tNo more messages to send." << std::endl;
				break;
			}
		}
		else
		{
			break;
		}
	}

}

bool Msg::operator==(const Msg& RHS)
{
	return (this->recipientID == RHS.recipientID &&
			this->message == RHS.message &&
			this->timeStamp == RHS.timeStamp &&
			this->delay == RHS.delay);
}


/************************************************************************/
/* Return true if this message will be sent later than RHS message      */
/************************************************************************/
bool Msg::operator<(const Msg& RHS) const
{
	return ((float)this->timeStamp / CLOCKS_PER_SEC + this->delay > (float)RHS.timeStamp / CLOCKS_PER_SEC + RHS.delay);
}
