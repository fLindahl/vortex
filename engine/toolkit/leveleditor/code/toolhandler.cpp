#include "toolhandler.h"
#include "basetool.h"
#include "selecttool.h"
#include "translatetool.h"
#include "rotatetool.h"

namespace Tools
{
	ToolHandler::ToolHandler()
	{
		this->selectTool = new Tools::SelectTool();
		this->translateTool = new Tools::TranslateTool();
		this->rotateTool = new Tools::RotateTool();
		this->currentTool = this->selectTool;
	}

	ToolHandler::~ToolHandler()
	{
		delete this->selectTool;
		delete this->translateTool;
		delete this->rotateTool;
	}

	Tools::BaseTool* ToolHandler::CurrentTool() const
	{
		return this->currentTool;
	}

	Tools::SelectTool* ToolHandler::SelectTool() const
	{
		return this->selectTool;
	}

	void ToolHandler::SetSelectTool()
	{
		this->currentTool = this->selectTool;
	}

	void ToolHandler::SetTranslateTool()
	{
		this->currentTool = this->translateTool;
	}

	void ToolHandler::SetRotateTool()
	{
		this->currentTool = this->rotateTool;
	}
}