#include "config.h"
#include "baseproperty.h"

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

//-----------------------------------
/**
	Called by the inspector in the level editor.
*/
void BaseProperty::Inspect()
{
	return;
}

}