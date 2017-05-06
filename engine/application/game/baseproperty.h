#pragma once
#include "core/refcounted.h"
#include "foundation/messaging/messagehandler.h"
#include "entity.h"

//---------------------------------------
/**
	@class	Game::BaseProperty

	
*/	
namespace Game
{

class BaseProperty : public Core::RefCounted
{
__DeclareClass(BaseProperty)
public:
	BaseProperty();
    virtual ~BaseProperty() {}

    virtual void FixedUpdate() {}
    virtual void Update() {}

	virtual void HandleMessage(const Ptr<Messaging::Message>& msg) {}

	virtual void Activate() { active = true; }
	virtual void Deactivate() { active = false; }

	void SetOwner(Ptr<Entity> o) { this->owner = o; }
	const Ptr<Entity>& GetOwner() { return this->owner; }

	//void Serialize(IO::Serializer* serializer);

protected:
	bool active = false;
    friend class Entity;

	///The owner entity of this property
    Ptr<Entity> owner;
};

}