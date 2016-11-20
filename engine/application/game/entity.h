#pragma once
#include "config.h"
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
#include "application/basegamefeature/managers/entitymanager.h"
#include "foundation/messaging/messagehandler.h"
#include "foundation/math/vector4.h"
#include "baseproperty.h"

namespace Game
{

class Entity
{
public:
	Entity();
	~Entity();

	int getID() { return ID; };
	//void setID(const int id) { ID = id; };

	virtual void Activate();
	virtual void Deactivate();

	virtual void FixedUpdate();
	virtual void Update();

	virtual void HandleMsg(std::shared_ptr<BaseGameFeature::Msg> msg);
	virtual void SendMsg(std::shared_ptr<BaseGameFeature::Msg> msg);
	virtual void SendMsg(const int& recipientID, const BaseGameFeature::MsgType& message, const float& delay);

    void AddProperty(std::shared_ptr<Game::BaseProperty> p);

	virtual Math::mat4 GetTransform();
	virtual void SetTransform(const Math::mat4& nTransform);

protected:
    //friend class BaseGameFeature::EntityManager;

    Math::mat4 transform;

	int ID;

	Util::Array<std::shared_ptr<Game::BaseProperty>> properties;
};

}