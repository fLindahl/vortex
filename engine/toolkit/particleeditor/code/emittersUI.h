#pragma once
#include <string>
#include <memory>
#include "render/particlesystem/particlesystem.h"
#include "core/ptr.h"
#include "imgui_color_gradient.h"

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
	std::string texture;

	bool visible = true;

	bool active = false;
	
};

namespace ParticleEditor
{
class EmittersUI
{
public:
	EmittersUI();
	EmittersUI(UserInterface* ui, int id, bool dummy = false);
	~EmittersUI();

	void DrawEmitter();

	EmitterValues ev;
	Particles::ParticleUISettings settings;

	int id;

	ImGradient gradient;

	ImGradientMark* draggingMark = nullptr;
	ImGradientMark* selectedMark = nullptr;
private:
	UserInterface* ui;

	Ptr<Render::TextureResource> dupIcon;
	Ptr<Render::TextureResource> deleteIcon;

	Ptr<Render::TextureResource> visibleIcon;
	Ptr<Render::TextureResource> notVisibleIcon;

	Ptr<Render::TextureResource> activeIcon;
	Ptr<Render::TextureResource> inactiveIcon;
	
	char* name;

	bool dummy;
};	
}



