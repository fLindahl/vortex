#include "config.h"
#include "entitymanager.h"
#include "GLFW/glfw3.h"
#include "foundation/messaging/messagehandler.h"

namespace BaseGameFeature
{

EntityManager::EntityManager()
{
	//UPDATE_MULTIPLIER = iniParser::getUpdateMultiplier();

	this->lastUpdateTime = clock();
	double lastTime = glfwGetTime();
}

uint EntityManager::GetNewEntityID()
{
	return this->entityCounter++;	
}

void EntityManager::RegisterEntity(std::shared_ptr<Game::Entity> entity)
{
	this->EntityList.insert(std::pair<int, std::shared_ptr<Game::Entity>>(entity->getID(), entity));
}

void EntityManager::UnregisterEntity(const int& ID)
{
	if (EntityList.size() > 0 && EntityList.count(ID) > 0)
	{
		EntityList.erase(ID);
	}
}

void EntityManager::Update()
{
	double currentTime = glfwGetTime();

	deltaTime = (float)fabs(currentTime - lastTime);
	//Check delta time and update if it exceeds update frequency
	if (TimeSinceLastUpdate() >= UPDATE_FREQUENCY)
	{
		//update last update time to current clock-ticks
		lastUpdateTime = clock();

		for (std::unordered_map<int, std::shared_ptr<Game::Entity>>::iterator it = EntityList.begin(); it != EntityList.end(); it++)
		{
			std::shared_ptr<Game::Entity> entity = it->second;
			entity->FixedUpdate();
		}
	}
	
	// update the messagehandler
	MsgHandler::Instance()->Update();

	for (std::unordered_map<int, std::shared_ptr<Game::Entity>>::iterator it = this->EntityList.begin(); it != EntityList.end(); it++)
	{
		std::shared_ptr<Game::Entity> entity = it->second;
		entity->Update();
	}

	lastTime = currentTime;
}

double EntityManager::DeltaTime()
{
	return deltaTime < 1.0 ? (deltaTime < 0.000001 ? 0.0 : deltaTime) : 1.0;
}

double EntityManager::TimeSinceLastUpdate()
{
	return (((float)clock() - lastUpdateTime) / CLOCKS_PER_SEC);
}

std::shared_ptr<Game::Entity> EntityManager::GetEntityByID(const uint& id)
{
	_assert(this->EntityList.count(id) > 0, "ERROR: No Entity with this ID exists!");
	return this->EntityList[id];
}

}