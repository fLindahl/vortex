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
		///Opens any modal windows if necessary
		void ModalWindows();

		///this is set by SetCurrentPropertynspector
		LevelEditor::BaseInspector* currentInspector;

		LevelEditor::GraphicsPropertyInspector* graphicsPropertyInspector;

		bool SetCurrentPropertyInspector(Ptr<Game::BaseProperty>& property);

		bool openModalAddProperty;
	};
}