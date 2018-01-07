#include "config.h"
#include "rendermessages.h"
#include "render/resources/meshresource.h"

namespace Msg
{
	__ImplementClass(Msg::SetMesh, 'SMsh', Messaging::Message)
	__ImplementMsgType(Msg::SetMesh)

	SetMesh::SetMesh()
	{
	}

	SetMesh::~SetMesh()
	{
	}

	void SetMesh::Set(const Ptr<Render::MeshResource>& t)
	{
		this->mesh = t;
	}

	const Ptr<Render::MeshResource>& SetMesh::Get() const
	{
		_assert2(this->mesh.isvalid(), "Mesh not set!");
		return this->mesh;
	}
}
