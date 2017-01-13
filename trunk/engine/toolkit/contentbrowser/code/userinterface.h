#pragma once
#include "GL/glew.h"
#include <string>
#include "foundation/math/point.h"
#include "foundation/util/array.h"

namespace ContentBrowser { class Application; }

namespace Render { struct ModelNode; }

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

		void ModalWindows();

		///Browses a directory and appends result to outArray.
		void BrowseDirectory(const char* directory, Util::Array<std::string>& outArray);
		void ImGuiListDirectory(const char* label, int* selectedItem, Util::Array<std::string>& list);
		
		void NewModel();
		void SaveModel(bool newPath = false);
		void NewSurface();

		bool openFilePopup = false;
		bool confirmNewModelPopup = false;
		bool surfaceBrowserPopup = false;
		bool meshBrowserPopup = false;
		std::string savePath;

		Render::ModelNode* selectedNode;

		///Texture handles
		GLuint browseButtonTextureHandle;
	};

}
