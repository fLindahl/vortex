#pragma once
#include "GL/glew.h"
#include <string>
#include "foundation/math/point.h"

namespace ContentBrowser { class Application; }

namespace Toolkit
{

	class UserInterface
	{
	public:
		UserInterface(ContentBrowser::Application* app);
		~UserInterface();

		void Run();

	private:
		ContentBrowser::Application* application;
		
		void RenderDocks();
		void ExecShortCuts();
		void ShowFileMenu();
		
		void NewModel();
		void SaveModel(bool newPath = false);
		void NewSurface();

		bool openFilePopup = false;

		bool confirmNewModelPopup = false;

		std::string savePath;

		///Texture handles
		GLuint browseButtonTextureHandle;
	};

}
