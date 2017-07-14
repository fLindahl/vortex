/*------------------/
Class Tools::SelectTool

Used for selecting objects
/------------------*/

#pragma once
#include "basetool.h"
#include "application/game/entity.h"

namespace Tools
{
	class SelectTool : public BaseTool
	{
	public:
		SelectTool();
		~SelectTool();

		void SetSelectedEntity(const Ptr<Game::Entity>& entity);
		const Ptr<Game::Entity>& GetSelectedEntity();

	private:
		Ptr<Game::Entity> selectedEntity;
	};
}