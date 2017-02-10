/*
 * TODO: Add Delete buttons for lights
 */

#pragma once
#include "GL/glew.h"
#include <string>
#include <render/server/lightserver.h>
#include "foundation/math/point.h"
#include "undo.h"

namespace LevelEditor { class Application; }

namespace Tools 
{
	class BaseTool;
	class SelectTool;
	class TranslateTool;
}

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

        /// Light Variables ///
		int light;
        Render::LightServer::LightType lT;

	};

}
