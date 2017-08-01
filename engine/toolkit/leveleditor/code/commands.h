#pragma once
#include "undo.h"

namespace Edit
{
	class AddEntity : public Command
	{
	public:
		Ptr<Game::Entity> entity;

	public:
		AddEntity(const Math::vec4& position, Ptr<Render::ModelInstance> mdl)
		{
			this->entity = Game::Entity::Create();
		}
		~AddEntity()
		{

		}

		virtual bool Execute()
		{
			this->entity->Activate();
			return true;
		}

		virtual bool Unexecute()
		{
			this->entity->Deactivate();
			return true;
		}

	};
}
