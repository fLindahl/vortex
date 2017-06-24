#pragma once
#include "foundation/util/string.h"

namespace Interface
{
	class InterfaceBase
	{
	public:
		InterfaceBase(Util::String name);
		virtual ~InterfaceBase() = 0;
		
		const Util::String& GetName() const;
		void SetName(const char* name);

		//Get and set so that ImGui can access it
		bool& Open();

	private:
		Util::String name;
		bool open;
	};

}