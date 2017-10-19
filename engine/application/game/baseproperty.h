#pragma once
#include "core/refcounted.h"
#include "foundation/messaging/messagehandler.h"
#include "entity.h"
#include "IO/serialization.h"

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

	virtual void Activate() { this->active = true; }
	virtual void Deactivate() { this->active = false; }

	///Override in subclass. Call this function with you favorite stream write/reader.
	virtual void Serialize(IO::Stream* serializer);

	const Ptr<Entity>& GetOwner() { return this->owner; }

	bool IsActive() const { return this->active; }

protected:
	bool active = false;
    friend class Entity;

	///Sets the owner of this property
	void SetOwner(Ptr<Entity> o) { this->owner = o; }
	

	///The owner entity of this property
    Ptr<Entity> owner;	
};

}