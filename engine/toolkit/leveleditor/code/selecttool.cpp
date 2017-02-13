#include "selecttool.h"

namespace Tools
{
	SelectTool::SelectTool()
	{
		type = ToolType::SELECT;
		this->currentHandle = TransformHandle::NONE;
	}

	SelectTool::~SelectTool()
	{
	}

}