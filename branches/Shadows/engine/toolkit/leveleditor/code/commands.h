#pragma once
#include "undo.h"
#include "application/game/modelentity.h"
#include "application/game/modelentityWithSpotlight.h"

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

    class AddSpotlightEntity : public Command
    {
    public:
        std::shared_ptr<Game::ModelEntitySpotLight> entity;

    public:
        AddSpotlightEntity(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
        {
            this->entity = std::make_shared<Game::ModelEntitySpotLight>();
            this->entity->SetModel(mdl);
            this->entity->SetTransform(Math::mat4::translation(position));

            /// Default values
            Render::SpotLight spotlight;
            spotlight.position = this->entity->GetTransform().get_position();
            spotlight.color = Math::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            spotlight.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
            spotlight.length = 5.0f;
            spotlight.angle = 15.0f;
            Render::LightServer::Instance()->AddSpotLight(spotlight);

            this->entity->SetSpotLightEnity(&Render::LightServer::Instance()->GetSpotLightAtIndex(Render::LightServer::Instance()->GetNumSpotLights() - 1));
            this->entity->SetIndex(Render::LightServer::Instance()->GetNumSpotLights() - 1);

        }
        ~AddSpotlightEntity()
        {
            //empty
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
