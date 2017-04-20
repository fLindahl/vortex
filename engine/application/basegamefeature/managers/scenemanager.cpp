#include "config.h"
#include "scenemanager.h"
#include "render/resources/meshresource.h"
#include "render/server/resourceserver.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "render/resources/surface.h"
#include "tinyxml2.h"
#include "application/game/geometryproxyentity.h"
#include "application/game/cubemapentity.h"
#include "envmanager.h"
#include "render/resources/geometryproxy.h"
#include "entitymanager.h"

namespace BaseGameFeature
{

SceneManager::SceneManager()
{

}

Ptr<Render::ModelInstance> SceneManager::LoadOBJScene(const char* filename, const char* mtlfile)
{
	// Load .mtl file!
	Util::Array<Ptr<Render::Surface>> surfaces;
	surfaces = Render::ResourceServer::Instance()->LoadMTLFile(mtlfile);
	

	Ptr<Render::ModelInstance> model = Render::ModelInstance::Create();
	model->SetMesh(filename);
	//model->SetSurfaceList(surfaces);
	

	return model;
}

bool SceneManager::LoadXMLScene(const char* filename)
{
	tinyxml2::XMLDocument data;
	int result = data.LoadFile(filename);

	if (result != 0)
	{
		printf("ERROR: Could not load .XML scene!");

#ifdef DEBUG
		assert(false);
#endif // DEBUG

		return false;
	}

	//geometry proxies and environment probes
	const tinyxml2::XMLElement* proxy = data.RootElement()->FirstChildElement("GeometryProxy");

	while (proxy != nullptr)
	{
		Ptr<Game::GeometryProxyEntity> proxyEntity = Game::GeometryProxyEntity::Create();
		//Set icon
		proxyEntity->SetModel(Render::ResourceServer::Instance()->LoadModel("resources/models/geometryproxy_icon.mdl"));

		Math::vector wPos;
		wPos.set_x(proxy->FindAttribute("x")->FloatValue());
		wPos.set_y(proxy->FindAttribute("y")->FloatValue());
		wPos.set_z(proxy->FindAttribute("z")->FloatValue());

		Math::vector scale;
		scale.set_x(proxy->FindAttribute("scalex")->FloatValue());
		scale.set_y(proxy->FindAttribute("scaley")->FloatValue());
		scale.set_z(proxy->FindAttribute("scalez")->FloatValue());

		proxyEntity->SetTransform(Math::mat4::translation(wPos));
		proxyEntity->SetScale(scale);

		proxyEntity->Activate();

		//Setup all environment probes that connects to this proxy
		const tinyxml2::XMLElement* probe = proxy->FirstChildElement("EnvironmentProbe");

		while (probe != nullptr)
		{
			Ptr<Game::CubeMapEntity> envProbeEntity = Game::CubeMapEntity::Create();
			envProbeEntity->SetModel(Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));

			wPos.set_x(probe->FindAttribute("x")->FloatValue());
			wPos.set_y(probe->FindAttribute("y")->FloatValue());
			wPos.set_z(probe->FindAttribute("z")->FloatValue());

			envProbeEntity->SetTransform(Math::mat4::translation(wPos));

			envProbeEntity->GetCubeMapNode()->InnerScale() = probe->FindAttribute("innerScale")->FloatValue();
			envProbeEntity->GetCubeMapNode()->OuterScale() = probe->FindAttribute("outerScale")->FloatValue();

			envProbeEntity->SetGeometryProxy(proxyEntity->GetGeometryProxy());

			envProbeEntity->Activate();

			probe = probe->NextSiblingElement("EnvironmentProbe");
		}

		proxy = proxy->NextSiblingElement("GeometryProxy");
	}

	return true;

}

bool SceneManager::SaveXMLScene(const char* filename)
{
	tinyxml2::XMLDocument data;
				
	tinyxml2::XMLNode* pRoot = data.NewElement("Vortex");
	data.InsertFirstChild(pRoot);

	uint numProxies = EnvManager::Instance()->geometryProxies.Size();

	for (int i = 0; i < numProxies; ++i)
	{
		auto proxyEntity = EnvManager::Instance()->geometryProxies[i];
		
		tinyxml2::XMLElement* proxy = data.NewElement("GeometryProxy");
		proxy->SetAttribute("x", proxyEntity->Transform().get_position().x());
		proxy->SetAttribute("y", proxyEntity->Transform().get_position().y());
		proxy->SetAttribute("z", proxyEntity->Transform().get_position().z());
		proxy->SetAttribute("scalex", proxyEntity->Transform().row0()[0]);
		proxy->SetAttribute("scaley", proxyEntity->Transform().row1()[1]);
		proxy->SetAttribute("scalez", proxyEntity->Transform().row2()[2]);

		uint numEnvProbes = proxyEntity->GetConnectedCubemaps().Size();
		for (int j = 0; j < numEnvProbes; ++j)
		{
			auto cubemap = proxyEntity->GetConnectedCubemaps()[j];

			tinyxml2::XMLElement* element = data.NewElement("EnvironmentProbe");
			element->SetAttribute("x", cubemap->GetPosition().x());
			element->SetAttribute("y", cubemap->GetPosition().y());
			element->SetAttribute("z", cubemap->GetPosition().z());
			element->SetAttribute("innerScale", cubemap->InnerScale().x());
			element->SetAttribute("outerScale", cubemap->OuterScale().x());
			
			proxy->InsertEndChild(element);
		}

		pRoot->InsertFirstChild(proxy);
	}

	auto entities = BaseGameFeature::EntityManager::Instance()->GetEntityList();

	for (auto pair : entities)
	{
		const Ptr<Game::Entity>& entity = pair.second;
		//entity->Serialize(data);
	}

	tinyxml2::XMLError eResult = data.SaveFile(filename);
	assert(!eResult);

	return true;
}

}