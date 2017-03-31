#pragma once
#include "core/refcounted.h"
#include "foundation/messaging/messagehandler.h"
#include "entity.h"

namespace Game
{

class BaseProperty : public Core::RefCounted
{
__DeclareClass(BaseProperty);
public:
    BaseProperty() {}
    virtual ~BaseProperty() {}

    virtual void FixedUpdate() {}
    virtual void Update() {}

    //void HandleMsg(const BaseGameFeature::Msg& msg);
	
	virtual void Activate() { active = true; }
	virtual void Deactivate() { active = false; }

	void SetOwner(Ptr<Entity> o) { this->owner = o; }
	Ptr<Entity> GetOwner() { return this->owner; }

protected:
	bool active = false;
    friend class Entity;

    Ptr<Entity> owner;
};

}