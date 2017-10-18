#pragma once
#include "undo.h"

namespace Edit
{
	class AddEntity : public Command
	{
	public:
		//Ptr<Game::CubeMapEntity> entity;

	public:
		/*
		AddEntity(const Math::vec4& position, Ptr<Render::ModelInstance> mdl)
		{
			this->entity = Game::CubeMapEntity::Create();
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
		*/
	};
/*
	class AddGeometryProxyEntity : public Command
	{
	public:
		Ptr<Game::GeometryProxyEntity> entity;

	public:
		AddGeometryProxyEntity(const Math::vec4& position, Ptr<Render::ModelInstance> mdl)
		{
			this->entity = Game::GeometryProxyEntity::Create();
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
        Ptr<Game::ModelEntitySpotLight> entity;

    public:
        AddSpotlightEntity(const Math::vec4& position, Ptr<Render::ModelInstance> mdl)
        {
            this->entity = Game::ModelEntitySpotLight::Create();
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
        Ptr<Game::PointLightEntity> entity;

    public:
        AddPointlightEntity(const Math::vec4& position, Ptr<Render::ModelInstance> mdl)
        {
            this->entity = Game::PointLightEntity::Create();
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

*/
}
