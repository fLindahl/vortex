#pragma once
#include "baseinspector.h"
#include "physics/boxcolliderproperty.h"

namespace LevelEditor
{
    class BoxColliderInspector : public BaseInspector
    {
    public:
		BoxColliderInspector();
        ~BoxColliderInspector();

        void DrawGUI();
        void Update();
        void SetProperty(const Ptr<Game::BaseProperty>& property);
        
		Ptr<Game::BaseProperty> CreateNewProperty();

    private:
        Ptr<Property::BoxCollider> property;		
    };
}