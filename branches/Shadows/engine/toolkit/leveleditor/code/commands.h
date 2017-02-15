#pragma once

#include <application/game/boxentity.h>
#include "undo.h"
#include "application/game/cubemapentity.h"
#include "application/game/modelentityWithSpotlight.h"
#include "application/game/pointlightentity.h"

namespace Edit
{
	class AddEntity : public Command
	{
	public:
		std::shared_ptr<Game::CubeMapEntity> entity;

	public:
		AddEntity(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
		{
			this->entity = std::make_shared<Game::CubeMapEntity>();
			this->entity->SetModel(mdl);
			this->entity->SetTransform(Math::mat4::translation(position));
		}
		~AddEntity()
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

    class AddBox : public Command
    {
      public:
        std::shared_ptr<Game::BoxEntity> entity;

      public:
        AddBox(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
        {
            this->entity = std::make_shared<Game::BoxEntity>();
            this->entity->SetModel(mdl);
            this->entity->SetTransform(Math::mat4::translation(position));
        }
        ~AddBox()
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
            Render::LightServer::Instance()->CreateSpotLight(Math::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                                             this->entity->GetTransform().get_position(),
                                                             Math::vec4(0.0f, -1.0f, 0.0f, 1.0f),
                                                             5.0f,
                                                             15.0f);

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
            Render::LightServer::Instance()->CreatePointLight(Math::vec4(0.0f, 0.0f, 1.0f, 1.0f), this->entity->GetTransform().get_position(), 3.0f);

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
