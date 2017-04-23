#pragma once
#include "core/refcounted.h"
#include "message.h"
#include <ctime>
#include <string>
#include <vector>
#include <queue>

namespace Messaging
{

class MsgHandler
{
public:
	static MsgHandler *Instance()
	{
		static MsgHandler instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	MsgHandler(MsgHandler const &) = delete;
	void operator=(MsgHandler const &) = delete;

	void RecvMsg(Message msg);
	void SendMsg(Message msg);
	void Update();

private:
	// Private constructor
	// Prevents duplicate singletons
	MsgHandler() {}

	// Sorted list of all current messages
	std::priority_queue<Message> msgQueue;

};

}