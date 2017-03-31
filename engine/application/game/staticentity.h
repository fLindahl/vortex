#pragma once

#include "entity.h"
#include "fysik/surfacecollider.h"

namespace Game
{
class StaticEntity : public PhysicsEntity
{
	__DeclareClass(StaticEntity)
public:
    StaticEntity();
    ~StaticEntity();

	///Registers this entity to the physics server
    void Activate();
    void Deactivate();

    void SetTransform(const Math::mat4& t);

private:

};

}