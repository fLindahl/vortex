#pragma once
#include "application.h"
#include <memory>

class UserInterface
{
public:
	UserInterface(std::shared_ptr<ParticleEditor::Application> app);
	~UserInterface();

	std::shared_ptr<ParticleEditor::Application> application;
};

