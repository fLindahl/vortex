#include "interface.h"

Interface::InterfaceBase::InterfaceBase(Util::String name) :
	name(name),
	open(false)
{
}

const Util::String & Interface::InterfaceBase::GetName() const
{
	return this->name;
}

void Interface::InterfaceBase::SetName(const char * name)
{
	this->name = name;
}

bool & Interface::InterfaceBase::Open()
{
	return this->open;
}
