#pragma once
#include "interface.h"

namespace Interface
{
	class SceneView : public InterfaceBase
	{
	__DeclareClass(SceneView)
	public:
		SceneView();
		~SceneView();

		void Run();

	private:
	};
}