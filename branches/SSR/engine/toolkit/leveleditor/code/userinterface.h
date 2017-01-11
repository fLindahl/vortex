#pragma once
#include "GL/glew.h"
#include <string>
#include "foundation/math/point.h"
#include "undo.h"
#include "tools/basetool.h"
#include "tools/selecttool.h"
#include "tools/translatetool.h"

namespace LevelEditor { class Application; }

namespace Toolkit
{

	class UserInterface
	{
	public:
		UserInterface(LevelEditor::Application* app);
		~UserInterface();

		void Run();

		double frameTime;
	private:
		Edit::CommandManager* commandManager;
		LevelEditor::Application* application;
		
		void RenderDocks();
		void ExecShortCuts();
		void ShowFileMenu();
		
		double prevFPSTime;
		double currentFPS;

		std::string consoleBuffer;
		std::string a;
		
		//Tools
		Tools::BaseTool* currentTool;
		
		Tools::SelectTool* selectTool;
		Tools::TranslateTool* translateTool;

		///Texture Handles
		//Toolbar
		GLuint selectToolTextureHandle;
		GLuint translateToolTextureHandle;
		GLuint rotateToolTextureHandle;
		GLuint scaleToolTextureHandle;
		GLuint entityToolTextureHandle;
		GLuint brushToolTextureHandle;
		GLuint polygonEditTextureHandle;


	};

}
