#pragma once
#include "undo.h"
#include "application/game/modelentity.h"
#include "render/server/lightserver.h"

namespace Edit
{
	class AddEntity : public Command
	{
	public:
		std::shared_ptr<Game::ModelEntity> entity;

	public:
		AddEntity(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
		{
			this->entity = std::make_shared<Game::ModelEntity>();
			this->entity->SetModel(mdl);
			this->entity->SetTransform(Math::mat4::translation(position));

			Render::SpotLight entitylight;
			entitylight.position = position;
			entitylight.color = Math::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			entitylight.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
			entitylight.length = 5.0f;
			entitylight.angle = 15.0f;
			Render::LightServer::Instance()->AddSpotLight(entitylight);
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