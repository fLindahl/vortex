#pragma once
#include "foundation/util/string.h"
#include "core/refcounted.h"
#include "imgui.h"
#include "imgui_dock.h"

namespace Interface
{
	class InterfaceBase : public Core::RefCounted
	{
		__DeclareClass(InterfaceBase)
	public:
		InterfaceBase();
		InterfaceBase(Util::String name);
		virtual ~InterfaceBase();
		
		const Util::String& GetName() const;
		void SetName(const char* name);

		//Get and set so that ImGui can access it
		bool& Open();

		//Runs and renders the interface once.
		virtual void Run();

		const ImGuiWindowFlags_& GetAdditionalFlags() const;

	protected:
		ImGuiWindowFlags_ additionalFlags;
		Util::String name;
		bool open;
	};

}