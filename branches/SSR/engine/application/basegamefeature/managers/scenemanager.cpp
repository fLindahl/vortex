#include "config.h"
#include "scenemanager.h"
#include "render/resources/meshresource.h"
#include "render/server/resourceserver.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "render/resources/surface.h"

namespace BaseGameFeature
{

std::shared_ptr<Render::ModelInstance> SceneManager::LoadOBJScene(const char* filename, const char* mtlfile)
{
	// Load .mtl file!
	Util::Array<std::shared_ptr<Render::Surface>> surfaces;
	surfaces = Render::ResourceServer::Instance()->LoadMTLFile(mtlfile);
	

	std::shared_ptr<Render::ModelInstance> model = std::make_shared<Render::ModelInstance>();
	model->SetMesh(filename);
	//model->SetSurfaceList(surfaces);
	

	return model;
}

}