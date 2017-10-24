#pragma once
#include "interface.h"
#include "../inspectors/baseinspector.h"
#include "../inspectors/graphicspropertyinspector.h"
#include "../inspectors/rigidbodyinspector.h"
#include "../inspectors/boxcolliderinspector.h"

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
		LevelEditor::RigidbodyInspector* rigidbodyInspector;
		LevelEditor::BoxColliderInspector* boxColliderInspector;

		Util::Array<LevelEditor::BaseInspector*> inspectors;

		bool SetCurrentPropertyInspector(Ptr<Game::BaseProperty>& property);

		bool openModalAddProperty;
	};
}