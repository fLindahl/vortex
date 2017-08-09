#include "browser.h"

namespace Interface
{
	__ImplementClass(Interface::Browser, 'Brws', Interface::InterfaceBase)

	Browser::Browser()
	{

	}

	Browser::~Browser()
	{

	}

	void Browser::Run()
	{
		if (ImGui::Button("Add CubeMap", { 100, 40 }))
		{
			//std::shared_ptr<Edit::AddEntity> command = std::make_shared<Edit::AddEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
			//commandManager->DoCommand(command);
		}

		if (ImGui::Button("Add Spotlight", { 125, 40 }))
		{
			//std::shared_ptr<Edit::AddSpotlightEntity> command = std::make_shared<Edit::AddSpotlightEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
			//commandManager->DoCommand(command);
		}

		if (ImGui::Button("Add Point Light", { 125, 40 }))
		{
			//std::shared_ptr<Edit::AddPointlightEntity> command = std::make_shared<Edit::AddPointlightEntity>(Graphics::MainCamera::Instance()->GetPosition(), Render::ResourceServer::Instance()->LoadModel("resources/models/cubemap_icon.mdl"));
			//commandManager->DoCommand(command);
		}
	}
}


