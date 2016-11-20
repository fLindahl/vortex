#pragma once

#include "entity.h"
#include "fysik/surfacecollider.h"

namespace Game
{
class StaticEntity : public PhysicsEntity
{
public:
    StaticEntity();
    ~StaticEntity();

    void Activate();
    void Deactivate();

    void SetTransform(const Math::mat4& t);

private:

};

}