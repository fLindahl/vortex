#pragma once
#include "foundation/messaging/message.h"

namespace Msg
{
	class SetTransform : public Messaging::Message
	{
	__DeclareClass(SetTransform)
	__DeclareMsgType
	public:
		SetTransform();
		~SetTransform();

		void Set(const Math::mat4& t);
		const Math::mat4& Get();

	private:
		const Math::mat4* transform;

	};
}