#pragma once
#include "undo.h"
#include "application/game/cubemapentity.h"
#include "application/game/modelentityWithSpotlight.h"
#include "application/game/pointlightentity.h"
#include "application/game/geometryproxyentity.h"

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

	class AddGeometryProxyEntity : public Command
	{
	public:
		std::shared_ptr<Game::GeometryProxyEntity> entity;

	public:
		AddGeometryProxyEntity(const Math::vec4& position, std::shared_ptr<Render::ModelInstance> mdl)
		{
			this->entity = std::make_shared<Game::GeometryProxyEntity>();
			this->entity->SetModel(mdl);
			this->entity->SetTransform(Math::mat4::translation(position));
		}
		~AddGeometryProxyEntity()
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
