#pragma once
#include "core/singleton.h"

namespace Tools
{
	//Forward declares
	class BaseTool;
	class SelectTool;
	class TranslateTool;
	class RotateTool;
	
	class ToolHandler
	{
		__DeclareSingleton(ToolHandler)
	public:
		//Returns the currently active tool
		Tools::BaseTool* CurrentTool() const;

		Tools::SelectTool* SelectTool() const;

		///Sets the currently active tool to the selection tool
		void SetSelectTool();
		///Sets the currently active tool to the translate tool
		void SetTranslateTool();
		///Sets the currently active tool to the rotate tool
		void SetRotateTool();


	private:
		///Cleans up memory allocations
		~ToolHandler();

		//Tools
		Tools::BaseTool* currentTool;

		Tools::SelectTool* selectTool;
		Tools::TranslateTool* translateTool;
		Tools::RotateTool* rotateTool;

	};
}