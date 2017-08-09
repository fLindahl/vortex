#include "config.h"
#include "graphicspropertyinspector.h"
#include "imgui.h"
#include "render/resources/modelinstance.h"
#include "render/server/resourceserver.h"

namespace LevelEditor
{
    GraphicsPropertyInspector::GraphicsPropertyInspector()
    {
        this->name = "Graphics Property";
    }
    
    GraphicsPropertyInspector::~GraphicsPropertyInspector()
    {
        //Empty
    }
	
    void GraphicsPropertyInspector::DrawGUI()
    {
        if(this->property == nullptr)
        {
            //Draw some indicator that something has gone wrong!
            return;
        }

        /*
        ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
        ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
        */

        BeginAttribute("Model");
        {
			if (ImGui::SmallButton("..."))
			{
				//Select Material for surface
			}
			ImGui::SameLine();
			if (ImGui::InputText("##mdlBuf", this->mdlInputBuf, ((int)(sizeof(this->mdlInputBuf) / sizeof(*this->mdlInputBuf))), ImGuiInputTextFlags_EnterReturnsTrue, nullptr, (void*)this))
			{
				auto mdl = Render::ResourceServer::Instance()->LoadModel(this->mdlInputBuf);
				if (mdl.isvalid())
					this->property->setModelInstance(mdl);
				else
					this->UpdateModelInputBuffer();
			}
			
			// keep focus previous widget if pressed
			if (ImGui::IsItemHovered() && (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && ImGui::IsMouseClicked(0)))
				ImGui::SetKeyboardFocusHere(-1); 
			
			/* List mesh nodes!
			ImGui::BeginChild("ModelNodes", ImVec2(0, 0), true);
			{
				//Unique identifier
				uint i = 2014235;
				for (auto node : mdl->GetModelNodes())
				{
					ImGui::BeginGroup();
					{
						std::string nodeName = mdl->GetMesh()->getPrimitiveGroup(node->primitiveGroup).name;

						ImGui::Text("Node: %s", nodeName.c_str());

						ImGui::Text("Surface: %s", node->surface->GetPath().AsCharPtr());
						ImGui::SameLine();
						ImGui::PushID(i++);
						if (ImGui::SmallButton("..."))
						{
							//Select Material for surface
						}
						ImGui::PopID();
						ImGui::Separator();
					}
					ImGui::EndGroup();
					
					if (ImGui::IsItemClicked())
					{

					}
				}
				ImGui::EndChild();
			}
			*/
			
        }
        EndAttribute();

        BeginAttribute("Floating point");
        {
            static float var = 0.2f;
            ImGui::InputFloat("##floatP", &var, 1.0f);
        }
        EndAttribute();

		BeginAttribute("Cast Shadows");
		{
			bool cast = this->property->GetCastShadows();

			if (ImGui::Checkbox("##castShdws", &cast))
			{
				this->property->SetCastShadows(cast);
			}
		}
		EndAttribute();
    }

    void GraphicsPropertyInspector::Update()
    {
        if(this->property == nullptr)
            return;
    }

    void GraphicsPropertyInspector::SetProperty(const Ptr<Game::BaseProperty>& property)
    {
        this->property = property.downcast<Render::GraphicsProperty>();

        if(this->property != nullptr)
        {
			this->UpdateModelInputBuffer();
        }
		else
		{
            _warning("Could not set inspector property to GraphicsProperty in GraphicsPropertyInspector::SetProperty!");
		}
    }

	Ptr<Game::BaseProperty> GraphicsPropertyInspector::CreateNewProperty()
	{
		Ptr<Render::GraphicsProperty> p = Render::GraphicsProperty::Create();
		return p;
	}

	void GraphicsPropertyInspector::UpdateModelInputBuffer()
	{
		//set input text to the selected model.
		if (this->property->getModelInstance() != nullptr)
			this->property->getModelInstance()->GetName().CopyToBuffer(this->mdlInputBuf, 128);
		else
			this->mdlInputBuf[0] = '\0'; //null-terminated
	}
}