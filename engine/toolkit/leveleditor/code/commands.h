#pragma once
#include "undo.h"
#include "application/game/rigidbodyentity.h"

namespace Edit
{
	class AddEntity : public Command
	{
	public:
		std::shared_ptr<Game::RigidBodyEntity> RBE;

	public:
		AddEntity(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
		{
			this->RBE = std::make_shared<Game::RigidBodyEntity>();
			this->RBE->SetModel(mdl);
			this->RBE->SetTransform(Math::mat4::translation(position));
		}
		~AddEntity()
		{

		}

		virtual bool Execute()
		{
			this->RBE->Activate();
			return true;
		}

		virtual bool Unexecute()
		{
			this->RBE->Deactivate();
			return true;
		}

	};
}