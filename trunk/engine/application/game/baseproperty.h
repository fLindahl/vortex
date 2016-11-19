#pragma once
#include "foundation/messaging/messagehandler.h"
#include "entity.h"

namespace Game
{
class BaseProperty
{
public:
    BaseProperty();
    ~BaseProperty();

    virtual void FixedUpdate();
    virtual void Update();

    //void HandleMsg(const BaseGameFeature::Msg& msg);

protected:
    friend class Entity;

    std::shared_ptr<Entity> owner;
};

}