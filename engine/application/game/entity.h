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

	uint GetID() { return ID; };
	//void SetID(const int id) { ID = id; };

	///Activates this entity and all it's properties
	virtual void Activate();
	///Deactivates this entity and all it's properties
	virtual void Deactivate();

	///Updates this entity and calls all its properties FixedUpdate() functions. This is called much less frequently than Update()
	virtual void FixedUpdate();
	///Updates this entity and calls all its properties Update() functions.
	virtual void Update();

	///Handles a message and dispatches it its properties
	virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
	
	///Append a property to this entity
    void AddProperty(const Ptr<Game::BaseProperty>& p);
	///Remove a property from this entity
	///Note: This calls the propertys destructor!
	void RemoveProperty(const Ptr<Game::BaseProperty>& p);

	///Shortcut for getting this entitys transform
	virtual Math::mat4 GetTransform();
	///Shortcut for setting this entitys transform
	virtual void SetTransform(const Math::mat4& nTransform);

	//virtual void Serialize();

protected:
	///Set to true when Activate is called. Set to false when Deactivat is called.
	bool active = false;

	///This entitys transform
    Math::mat4 transform;

	///Unique ID for this entity.
	uint ID;

	///List of all this netitys properties.
	Util::Array<Ptr<Game::BaseProperty>> properties;
};

}