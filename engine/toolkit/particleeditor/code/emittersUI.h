#pragma once
#include <string>
#include <memory>
#include "render/particlesystem/particlesystem.h"
#include "core/ptr.h"

class UserInterface;
namespace Render{ class TextureResource; }

struct EmitterValues
{
	EmitterValues()
	{
		name.reserve(512);
		name = "New Emitter";
	}
	std::string name;
	bool visible = true;

	bool active = false;
};

namespace ParticleEditor
{
class EmittersUI
{
public:
	EmittersUI();
	EmittersUI(std::shared_ptr<UserInterface> ui, int id);
	~EmittersUI();

	void DrawEmitter();

	EmitterValues ev;
	Particles::ParticleUISettings settings;

private:
	std::shared_ptr<UserInterface> ui;

	Ptr<Render::TextureResource> dupIcon;
	Ptr<Render::TextureResource> deleteIcon;

	Ptr<Render::TextureResource> visibleIcon;
	Ptr<Render::TextureResource> notVisibleIcon;

	Ptr<Render::TextureResource> activeIcon;
	Ptr<Render::TextureResource> inactiveIcon;
	
	char* name;
	int id;
};	
}



