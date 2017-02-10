#pragma once
#include "undo.h"
#include "application/game/modelentity.h"
#include "application/game/modelentityWithSpotlight.h"
#include "application/game/pointlightentity.h"

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

            /// Default values ///
            Render::LightServer::SpotLight light;
            light.position = this->entity->GetTransform().get_position();
            light.color = Math::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            light.coneDirection = Math::vec4(0.0f, -1.0f, 0.0f, 1.0f);
            light.length = 5.0f;
            light.angle = 15.0f;
            Render::LightServer::Instance()->AddSpotLight(light);

            this->entity->SetSpotLightEnity(&Render::LightServer::Instance()->GetSpotLightAtIndex((uint)Render::LightServer::Instance()->GetNumSpotLights() - 1));
            this->entity->SetLightIndex((uint)Render::LightServer::Instance()->GetNumSpotLights() - 1);
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

    class AddPointlightEntity : public Command
    {
    public:
        std::shared_ptr<Game::PointLightEntity> entity;

    public:
        AddPointlightEntity(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
        {
            this->entity = std::make_shared<Game::PointLightEntity>();
            this->entity->SetModel(mdl);
            this->entity->SetTransform(Math::mat4::translation(position));

            /// Default values ///
            Render::LightServer::PointLight light;
            light.position = this->entity->GetTransform().get_position();
            light.color = Math::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            light.radiusAndPadding.x() = 3.0f;
            Render::LightServer::Instance()->AddPointLight(light);

            this->entity->SetPointLightEnity(&Render::LightServer::Instance()->GetPointLightAtIndex((uint)Render::LightServer::Instance()->GetNumPointLights() - 1));
            this->entity->SetLightIndex((uint)Render::LightServer::Instance()->GetNumPointLights() - 1);
        }
        ~AddPointlightEntity()
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
