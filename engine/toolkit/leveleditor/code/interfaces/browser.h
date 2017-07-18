#pragma once
#include "interface.h"

namespace Interface
{
	class Browser : public InterfaceBase
	{
		__DeclareClass(Browser)
	public:
		Browser();
		~Browser();

		void Run();

	private:

	};
}