#pragma once
#include "foundation/messaging/message.h"

namespace Render
{
	class MeshResource;
}

namespace Msg
{
	class SetMesh : public Messaging::Message
	{
		__DeclareClass(SetMesh)
		__DeclareMsgType
	public:
		SetMesh();
		~SetMesh();

		void Set(Ptr<Render::MeshResource>& t);
		const Ptr<Render::MeshResource>& Get() const;

	private:
		Ptr<Render::MeshResource> mesh;

	};
}