 # Level Editor Inspectors

To inspect entities and their properties we use something called inspectors.
The idea for these came from the fact that we usually have huge systems that some properties depend on as long as the property is not a simple script (to be implemented...) and thus we usually need some specific way of presenting and interacting with the properties and their attributes within the level editor.

Essentially, an inspector is a hand-written serialization method for specific properties that presents and interacts with the properties using Ocurnut's Imgui. When scripts are implemented, the script-inspector will be automatically filled with attributes etc. based on the script itself. Only C++ properties will need their own inspectors. This gives us some creative freedom too by not having to rely on some ridiculous preprocessing macros or overhead like .nidl files. It does however force us to actually spend some time writing the inspectors (obviously).

## How to create an Inspector
First of, remeber that properties should never depend on Inspectors because we don't want to depend on the Level Editor project for our released games as that would just be unnecessary overhead.

In this example, let's say that we're creating a GraphicsProperty Inspector.

So, to create a new Inspector we need to create `graphicspropertyinspector.h` and `graphicspropertyinspector.cpp`.
### Header file
The header-file should look like this:

    #pragma once
    #include "baseinspector.h"
    //This is our property's header
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
            Ptr<Game::BaseProperty> CreateNewProperty();

        private:
            Ptr<Render::GraphicsProperty> property;            
        };
    }

##### Functions:
* `void DrawGUI();` - This is where we place our ImGui commands.

* `void Update();` - This is an additional function that is called right before drawing. This can be used to update some member variables that the drawing function depends on. This is not entierly necessary to override.

* `void SetProperty(const Ptr<Game::BaseProperty>& property);` - This function should only set the `property` member-variable to the argument provided and log some error if it fails.

* `Ptr<Game::BaseProperty> CreateNewProperty();` - This function is used when creating a new property to append to an entity. This is very important that you override.

### Source file

    #include "config.h"
    #include "graphicspropertyinspector.h"
    #include "imgui.h"

    namespace LevelEditor
    {
        GraphicsPropertyInspector::GraphicsPropertyInspector()
        {
            //This is the name that's displayed in the inspector
            this->name = "Graphics Property";
        }
        
        GraphicsPropertyInspector::~GraphicsPropertyInspector()
        {
            //Empty for now
        }
        
        void GraphicsPropertyInspector::DrawGUI()
        {
            // BeginAttribute names the field
            BeginAttribute("Model");
            {
                if (ImGui::SmallButton("Select Model..."))
                {
                    //Omitted Code
                }                
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
            //Empty for now
        }

        void GraphicsPropertyInspector::SetProperty(const Ptr<Game::BaseProperty>& property)
        {
            this->property = property.downcast<Render::GraphicsProperty>();            
        }

        Ptr<Game::BaseProperty> GraphicsPropertyInspector::CreateNewProperty()
        {
            Ptr<Render::GraphicsProperty> p = Render::GraphicsProperty::Create();
            return p;
        }
    }

This is essentially it for the source file.

* `BeginAttribute(const char*)` is how you add a field in which you place the functionality for the different attributes that the property has.

### Register as an inspector
This part is still under construction, but currently you need to add the inspector to `interfaces/inspector.cpp` in the constructor:

Ex.

    Inspector::Inspector()
	{
		//Do this for every inspector!
		//-----
		this->graphicsPropertyInspector = new LevelEditor::GraphicsPropertyInspector();
		this->inspectors.Append(this->graphicsPropertyInspector);
		//-----
		this->rigidbodyInspector = new LevelEditor::RigidbodyInspector();
		this->inspectors.Append(this->rigidbodyInspector);
		//-----

	}

and also add it to the `SetCurrentPropertyInspector` function:

Ex.

    bool Inspector::SetCurrentPropertyInspector(Ptr<Game::BaseProperty>& property)
	{
		if (property->IsA(Render::GraphicsProperty::RTTI))
		{
			this->currentInspector = graphicsPropertyInspector;
		}
		else if(property->IsA(Property::Rigidbody::RTTI))
		{
			this->currentInspector = rigidbodyInspector;
		}
		else
		{
			Util::String msg;
			msg.Format("Could not find inspector for property called \"%s\"", property->GetClassName().AsCharPtr());
			IO::Console::Instance()->Print(msg, IO::WARNING, true);
			return false;
		}

		this->currentInspector->SetProperty(property);
		return true;
	}

This will MOST DEFINETLY be reworked later but it works for now.