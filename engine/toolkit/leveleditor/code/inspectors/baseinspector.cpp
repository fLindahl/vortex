#include "config.h"
#include "baseinspector.h"
#include "imgui.h"

namespace LevelEditor
{
	BaseInspector::BaseInspector()
	{
	}

	BaseInspector::~BaseInspector()
	{
	}

	void BaseInspector::DrawGUI()
	{
		
	}

	void BaseInspector::Update()
	{

	}

	void BaseInspector::SetProperty(const Ptr<Game::BaseProperty>& property)
	{
		//Override in subclass
	}

	bool BaseInspector::IsModified() const
	{
		return isModified;
	}

	const Util::String& BaseInspector::GetName() const
	{
		return this->name;
	}

	void BaseInspector::BeginAttribute(const char* variableName)
	{
		ImGui::NextColumn();
		ImGui::AlignFirstTextHeightToWidgets();
		// Here we use a Selectable (instead of Text) to highlight on hover
		ImGui::Bullet();
		ImGui::Selectable(variableName);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
	}

	void BaseInspector::EndAttribute()
	{
		ImGui::PopItemWidth();
	}
}