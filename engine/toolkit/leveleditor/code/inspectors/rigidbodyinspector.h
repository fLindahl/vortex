#pragma once
#include "baseinspector.h"
#include "physics/rigidbodyproperty.h"

namespace LevelEditor
{
    class RigidbodyInspector : public BaseInspector
    {
    public:
		RigidbodyInspector();
        ~RigidbodyInspector();

        void DrawGUI();
        void Update();
        void SetProperty(const Ptr<Game::BaseProperty>& property);
        
		Ptr<Game::BaseProperty> CreateNewProperty();

    private:
        Ptr<Property::Rigidbody> property;		
    };
}