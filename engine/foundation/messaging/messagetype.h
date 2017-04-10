#pragma once
/**
	@class MessageType

	Used to identify messages by type
*/

namespace Messaging
{
class MessageType
{
public:
	/// constructor
	MessageType() {}
	/// equality operator
	bool operator==(const MessageType& rhs) const
	{
		return (this == &rhs);
	}
};

}