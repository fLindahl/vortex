#pragma once

namespace Render
{
	class ModelInstance;
}

namespace BaseGameFeature
{

class SceneManager
{

private:
	SceneManager() {}

public:
	static SceneManager* Instance()
	{
		static SceneManager instance;
		return &instance;
	}

	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	///Loads an .obj scene into the game. Make sure to only use this once per .obj/.mtl file!
	std::shared_ptr<Render::ModelInstance> LoadOBJScene(const char* filename, const char* mtlfile);

	//Loads an XML file scene
	bool LoadXMLScene(const char* filename);
	//Save an XML file scene
	bool SaveXMLScene(const char* filename);
private:


};

}