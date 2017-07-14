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
			//Lambda
			//auto SetModel = []() 
			//{
			//};

			if (ImGui::InputText("##mdlBuf", this->mdlInputBuf, ((int)(sizeof(this->mdlInputBuf) / sizeof(*this->mdlInputBuf))), ImGuiInputTextFlags_EnterReturnsTrue, nullptr, (void*)this))
			{
				auto mdl = Render::ResourceServer::Instance()->LoadModel(this->mdlInputBuf);

				if (mdl.isvalid())
				{
					this->property->setModelInstance(mdl);
				}
				else
				{
					//Reset input text to the selected model.
					if (this->property->getModelInstance() != nullptr)
						this->property->getModelInstance()->GetName().CopyToBuffer(this->mdlInputBuf, 128);
					else
						this->mdlInputBuf[0] = '\0'; //null-terminated
				}
			}
			if (ImGui::IsItemHovered() && (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && ImGui::IsMouseClicked(0)))
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			
        }
        EndAttribute();

        BeginAttribute("Floating point");
        {
            static float var = 0.2f;
            ImGui::InputFloat("##floatP", &var, 1.0f);
        }
        EndAttribute();

        
        ImGui::NextColumn();
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
			//set input text to the selected model.
			if (this->property->getModelInstance() != nullptr)
				this->property->getModelInstance()->GetName().CopyToBuffer(this->mdlInputBuf, 128);
			else
				this->mdlInputBuf[0] = '\0'; //null-terminated
        }
		else
		{
            _warning("Could not set inspector property to GraphicsProperty in GraphicsPropertyInspector::SetProperty!");
		}
    }
}