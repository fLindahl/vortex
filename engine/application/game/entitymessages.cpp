#include "config.h"
#include "entitymessages.h"

namespace Msg
{
	__ImplementClass(Msg::SetTransform, 'MSTr', Messaging::Message)
	__ImplementMsgType(Msg::SetTransform)

	SetTransform::SetTransform()
	{
	}

	SetTransform::~SetTransform()
	{
	}

	void SetTransform::Set(const Math::mat4& t)
	{
		this->transform = &t;
	}

	const Math::mat4& SetTransform::Get()
	{
		_assert2(this->transform != nullptr, "Transform not set!");
		return *this->transform;
	}
}