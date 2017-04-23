#pragma once
#include "core/refcounted.h"
#include "messagetype.h"

//-- MACROS -----------------------------------------
#define __DeclareMsgType \
public:\
    static Messaging::MessageType Type; \
    virtual const Messaging::MessageType& GetType() const;\
private:

#define __ImplementMsgType(classname) \
    Messaging::MessageType classname::Type; \
    const Messaging::MessageType& classname::GetType() const { return classname::Type; }

namespace Messaging
{

//---------------------------------------------------
/**
	@class	Messaging::Message
*/
class Message : public Core::RefCounted
{
	__DeclareClass(Message)
	__DeclareMsgType
public:
	Message() {}
	~Message() {}

	void SetHandled(bool val);
	bool IsHandled();

	void SetDistribute(bool val);
	bool IsDistributed();

private:

	///Is this message handled by an entity?
	bool handled;

	///Should this message be network distributed?
	bool distribute;
};

}