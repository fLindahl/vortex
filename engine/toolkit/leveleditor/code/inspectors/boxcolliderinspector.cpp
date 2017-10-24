#include "config.h"
#include "boxcolliderinspector.h"
#include "imgui.h"

namespace LevelEditor
{
	BoxColliderInspector::BoxColliderInspector()
    {
        this->name = "Box Collider";
    }
    
	BoxColliderInspector::~BoxColliderInspector()
    {
        //Empty
    }
	
    void BoxColliderInspector::DrawGUI()
    {
        if(this->property == nullptr)
        {
            //Draw some indicator that something has gone wrong!
            return;
        }
		
		
		BeginAttribute("Size");
		{
			static Math::vector var = this->property->GetSize();
			if (ImGui::InputFloat("##floatP", &var.x(), 0.05f))
			{
				this->property->SetSize(var);
			}
		}
		EndAttribute();
		

    }

    void BoxColliderInspector::Update()
    {
        if(this->property == nullptr)
            return;
    }

    void BoxColliderInspector::SetProperty(const Ptr<Game::BaseProperty>& property)
    {
        this->property = property.downcast<Property::BoxCollider>();

        if(this->property != nullptr)
        {
			
        }
		else
		{
            _warning("Could not set inspector property to BoxCollider in BoxColliderInspector::SetProperty!");
		}
    }

	Ptr<Game::BaseProperty> BoxColliderInspector::CreateNewProperty()
	{
		return Property::BoxCollider::Create();
	}

}