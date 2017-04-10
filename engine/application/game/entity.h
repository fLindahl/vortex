#pragma once
#include "config.h"
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
#include "application/basegamefeature/managers/entitymanager.h"
#include "foundation/messaging/messagehandler.h"
#include "foundation/math/vector4.h"
#include "entitymessages.h"

namespace Game
{
	class BaseProperty;
}

#define __SendMsg(OBJ, MSG) OBJ->HandleMessage(MSG.downcast<Messaging::Message>())

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

	virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
	
    void AddProperty(const Ptr<Game::BaseProperty>& p);
	void RemoveProperty(const Ptr<Game::BaseProperty>& p);

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