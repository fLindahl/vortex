#pragma once
#include "config.h"
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
#include "application/basegamefeature/managers/entitymanager.h"
#include "foundation/messaging/messagehandler.h"
#include "foundation/math/vector4.h"
#include "render/server/lightserver.h"

namespace Game
{
	class BaseProperty;
}

namespace Game
{
class Entity : public Core::RefCounted
{

	__DeclareClass(Entity);
public:
	Entity();
	~Entity();

	uint getID() { return ID; };
	//void setID(const int id) { ID = id; };

	virtual void Activate();
	virtual void Deactivate();

	virtual void FixedUpdate();
	virtual void Update();

	virtual void HandleMsg(Ptr<BaseGameFeature::Msg> msg);
	virtual void SendMsg(Ptr<BaseGameFeature::Msg> msg);
	virtual void SendMsg(const int& recipientID, const BaseGameFeature::MsgType& message, const float& delay);

    void AddProperty(Ptr<Game::BaseProperty> p);

	virtual Math::mat4 GetTransform();
	virtual void SetTransform(const Math::mat4& nTransform);

protected:
    //friend class BaseGameFeature::EntityManager;

	bool active = false;

    Math::mat4 transform;

	uint ID;

	Util::Array<Ptr<Game::BaseProperty>> properties;
};

}