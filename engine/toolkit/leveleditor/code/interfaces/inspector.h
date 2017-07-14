#pragma once
#include "interface.h"
#include "../inspectors/baseinspector.h"
#include "../inspectors/graphicspropertyinspector.h"

namespace Interface
{
	class Inspector : public InterfaceBase
	{
		__DeclareClass(Inspector)
	public:
		Inspector();
		~Inspector();

		void Run();

	private:
		LevelEditor::GraphicsPropertyInspector* graphicsPropertyInspector;

	};
}