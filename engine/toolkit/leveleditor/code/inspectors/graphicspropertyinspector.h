#pragma once
#include "baseinspector.h"
#include "render/properties/graphicsproperty.h"

namespace LevelEditor
{
    class GraphicsPropertyInspector : public BaseInspector
    {
    public:
        GraphicsPropertyInspector();
        ~GraphicsPropertyInspector();

        void DrawGUI();
        void Update();
        void SetProperty(const Ptr<Game::BaseProperty>& property);
            
		
    private:
        Ptr<Render::GraphicsProperty> property;
		
		char mdlInputBuf[128];
		void UpdateModelInputBuffer();
		
    };
}