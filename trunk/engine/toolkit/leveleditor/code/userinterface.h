#pragma once
#include "GL/glew.h"
#include <string>
#include "foundation/math/point.h"
#include "undo.h"

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
