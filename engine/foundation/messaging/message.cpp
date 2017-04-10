#include "config.h"
#include "message.h"

namespace Messaging
{
__ImplementClass(Messaging::Message, 'EMSG', Core::RefCounted)
__ImplementMsgType(Messaging::Message)

void Message::SetHandled(bool val)
{
	this->handled = val;
}

bool Message::IsHandled()
{
	return this->handled;
}

void Message::SetDistribute(bool val)
{
	this->distribute = val;
}

bool Message::IsDistributed()
{
	return this->distribute;
}

} // namespace Messaging