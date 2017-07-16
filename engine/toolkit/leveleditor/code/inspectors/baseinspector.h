#pragma once
#include "application/game/baseproperty.h"

namespace LevelEditor
{
	class BaseInspector
	{
	public:
		BaseInspector();
		~BaseInspector();

		///Draw the GUI
		virtual void DrawGUI() = 0;

		///Update the inspector and check for changes
		virtual void Update() = 0;

		virtual void SetProperty(const Ptr<Game::BaseProperty>& property) = 0;

		///Returns whether the object has been modified within the inspectors
		bool IsModified() const;

		const Util::String& GetName() const;

	protected:
		///Name of the property. Is shown on the property tab. Set in constructor of derived class
		Util::String name;
		///Set to true if any property has been modified.
		bool isModified;

		///Adds an attribute to the drawing list. Remember to call EndAttribute too!
		void BeginAttribute(const char* variableName);

		///Ends and attribute entry
		void EndAttribute();

	};

}