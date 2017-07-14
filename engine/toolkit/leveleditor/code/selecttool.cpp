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

	void SelectTool::SetSelectedEntity(const Ptr<Game::Entity>& entity)
	{
		this->selectedEntity = entity;
	}

	const Ptr<Game::Entity>& SelectTool::GetSelectedEntity()
	{
		return this->selectedEntity;
	}
}