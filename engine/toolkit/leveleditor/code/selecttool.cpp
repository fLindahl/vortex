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

	const Ptr<Game::Entity>& SelectTool::GetSelectedEntity()
	{
		return this->selectedEntity;
	}
}