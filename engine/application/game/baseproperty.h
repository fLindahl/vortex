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

    void SetOwner(Entity* o) {this->owner = o;}

protected:
    friend class Entity;

    Entity* owner;
};

}