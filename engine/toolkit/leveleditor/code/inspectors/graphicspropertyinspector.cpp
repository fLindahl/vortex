#include "config.h"
#include "graphicspropertyinspector.h"
#include "imgui.h"

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
            static float var = 0.0f;
            ImGui::InputFloat("##value", &var, 1.0f);
        }
        EndAttribute();

        BeginAttribute("Floating point");
        {
            static float var = 0.2f;
            ImGui::InputFloat("##value", &var, 1.0f);
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

        if(this->property == nullptr)
        {
            _warning("Could not set inspector property to GraphicsProperty in GraphicsPropertyInspector::SetProperty!");
        }
    }
}