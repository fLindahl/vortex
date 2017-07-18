#include "interface.h"

namespace Interface
{
	__ImplementClass(Interface::InterfaceBase, 'IfBs', Core::RefCounted)

	InterfaceBase::InterfaceBase() :
		additionalFlags((ImGuiWindowFlags_)0),
		name("NAME NOT SET"),
		open(false)
	{		
	}

	InterfaceBase::InterfaceBase(Util::String name) :
		name(name),
		open(false)
	{
	}

	InterfaceBase::~InterfaceBase()
	{
	}

	const Util::String & InterfaceBase::GetName() const
	{
		return this->name;
	}

	void InterfaceBase::SetName(const char * name)
	{
		this->name = name;
	}

	bool & InterfaceBase::Open()
	{
		return this->open;
	}

	void InterfaceBase::Run()
	{
		return;
	}

	const ImGuiWindowFlags_ & InterfaceBase::GetAdditionalFlags() const
	{
		return this->additionalFlags;
	}

}