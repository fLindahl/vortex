#pragma once

namespace Netcode
{

#define DEFAULT_PORT "49152"
#define DEFAULT_BUFLEN 512
#define MAXNAMELEN 32
	
	////////////////////
	// ENUMS & CONSTANTS
	////////////////////
	enum ObjectDesc
	{
		Human,
		NonHuman,
		Vehicle,
		StaticObject
	};

	enum ObjectForm
	{
		Cube,
		Sphere,
		Pyramid,
		Cone
	};

	struct Coordinate
	{
		int	x;
		int y;
	};
	
	///////////////
	// MESSAGE HEAD
	///////////////
	enum MsgType
	{
		Join,
		Leave,
		Change,
		Event,
		TextMessage
	};

	//included first in all messages
	struct MsgHead
	{
		unsigned int length;
		unsigned int seq_no;
		unsigned int id;
		MsgType	type;
	};

	////////////////////
	// MESSAGE TYPE JOIN
	////////////////////
	struct JoinMsg
	{
		MsgHead		head;
		ObjectDesc	desc;
		ObjectForm	form;
		char		name[MAXNAMELEN]; //nullterminated!, or empty
	};

	/////////////////////
	// MESSAGE TYPE LEAVE
	/////////////////////
	struct LeaveMsg
	{
		MsgHead head;
	};
}