#include "config.h"
#include "baseproperty.h"
#include "entity.h"

namespace Game
{

__ImplementClass(Game::BaseProperty, 'bpro', Core::RefCounted)

BaseProperty::BaseProperty()
{
	//Empty
}

void BaseProperty::Serialize(IO::Stream* stream)
{
	return;
}

const Entity* BaseProperty::GetOwner() const
{
	return this->owner;
}

void BaseProperty::SetOwner(Entity* o) 
{ 
	this->owner = o; 
}


}