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
		///this is set by SetCurrentPropertynspector
		LevelEditor::BaseInspector* currentInspector;

		LevelEditor::GraphicsPropertyInspector* graphicsPropertyInspector;
		void SetCurrentPropertyInspector(Ptr<Game::BaseProperty>& property);
	};
}