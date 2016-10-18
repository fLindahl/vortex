#pragma once
#include <ctime>
#include <string>
#include <vector>
#include <queue>

enum MsgType
{
	GOIDLE,
	HARVESTWOOD,
	UPGRADE_EXPLORER,
	UPGRADE_BUILDER,
	UPGRADE_COALWORKER,
	FOUNDWOOD,
	NEEDBUILDER,
	NEEDCOALWORKER
};

struct Msg
{
	MsgType message;
	int recipientID;
	int senderID;
	float delay; // seconds to delay this message
	clock_t timeStamp = clock();
	bool operator==(const Msg& RHS);
	bool operator<(const Msg& RHS) const;
};

class MsgHandler
{
public:
	static MsgHandler* getInstance()
	{
		static MsgHandler instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	MsgHandler(MsgHandler const&) = delete;
	void operator=(MsgHandler const&) = delete;

	void RecvMsg(Msg msg);
	void SendMsg(Msg msg);
	void Update();

private:
	// Private constructor
	// Prevents duplicate singletons
	MsgHandler() {}
	
	// Sorted list of all current messages
	std::priority_queue<Msg> msgQueue;
	
};
