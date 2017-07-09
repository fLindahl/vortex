#pragma once

namespace LevelEditor
{
	class BaseInspector
	{
	public:
		BaseInspector();
		~BaseInspector();

		///Draw the GUI
		void DrawGUI();

		///Update the inspector and check for changes
		void Update();

		///Returns whether the object has been modified within the inspectors
		bool IsModified() const;

	private:
		bool isModified;

	};

}