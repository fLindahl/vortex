#pragma once
#include "foundation/messaging/messagehandler.h"
#include "entity.h"

namespace Game
{
class Entity;

class BaseProperty
{
public:
    BaseProperty() {}
    virtual ~BaseProperty() {}

    virtual void FixedUpdate() {}
    virtual void Update() {}

    //void HandleMsg(const BaseGameFeature::Msg& msg);
	
	virtual void Activate() { active = true; }
	virtual void Deactivate() { active = false; }

	void SetOwner(std::shared_ptr<Entity> o) { this->owner = o; }

protected:
	bool active = false;
    friend class Entity;

    std::shared_ptr<Entity> owner;
};

}