#include "config.h"
#include "interfacemanager.h"

namespace Interface
{
	InterfaceManager::InterfaceManager()
	{

	}

	void InterfaceManager::RunAll()
	{
		for (auto it : this->interfaces)
		{
			it->Run();
		}
	}

	void InterfaceManager::RegisterInterface(const Util::String & className, const char * label)
	{
		Ptr<Core::RefCounted> ptr = Core::Factory::Instance()->Create(className);
		Ptr<InterfaceBase> intFace = ptr.downcast<InterfaceBase>();
		_assert2(intFace != nullptr, "Interface could not be found by provided class name!");

		intFace->SetName(label);
		this->interfaces.Append(intFace);
	}

	void InterfaceManager::RegisterInterface(const Util::FourCC fourcc, const char * label)
	{
		Ptr<Core::RefCounted> ptr = Core::Factory::Instance()->Create(fourcc);
		Ptr<InterfaceBase> intFace = ptr.downcast<InterfaceBase>();
		_assert2(intFace != nullptr, "Interface could not be found by provided FourCC");

		intFace->SetName(label);
		this->interfaces.Append(intFace);
	}

	void InterfaceManager::RegisterInterface(Ptr<InterfaceBase> base)
	{
		this->interfaces.Append(base);
	}

}