#pragma once
#include "core/refcounted.h"
#include "core/singleton.h"
#include "interfaces/interface.h"
#include "foundation/util/fourcc.h"

namespace Interface
{
	class InterfaceManager
	{
		__DeclareSingleton(InterfaceManager)
	public:
		void RunAll();

		///Register an interface by class name (RTTI)
		void RegisterInterface(const Util::String& className, const char* label);
		///Register an interface by fourcc
		void RegisterInterface(const Util::FourCC fourcc, const char* label);
		///Register an interface by pointer
		void RegisterInterface(const Ptr<InterfaceBase> base);

	private:
		Util::Array<Ptr<InterfaceBase>> interfaces;
	};

}