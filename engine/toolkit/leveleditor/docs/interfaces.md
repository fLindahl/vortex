# Level Editor Interfaces

Vortex Level Editor uses something we like to call interfaces to create dockable windows.
An interface is essentially just an object that holds all the rendering information that the InterfaceManager need to render the window.

## Creating an interface

To create an interface, start by creating the .h and .cpp files that will hold the interface subclass.

Your header-file should look something like this:

    #pragma once
    #include "interface.h"

    namespace Interface
    {
        class <SUBCLASSNAME> : public InterfaceBase
        {
            __DeclareClass(<SUBCLASSNAME>)
        public:
            <SUBCLASSNAME>();
            ~<SUBCLASSNAME>();

            void Run();

        private:

        };
    }

The source-file should look a little something like this:

    #include "<HEADERFILE>.h"

    namespace Interface
    {
        __ImplementClass(Interface::<SUBCLASSNAME>, '<FOUR CHARACTER CODE>', Interface::InterfaceBase)

        <SUBCLASSNAME>::<SUBCLASSNAME>()
        {
            
        }

        <SUBCLASSNAME>::~<SUBCLASSNAME>()
        {

        }

        void <SUBCLASSNAME>::Run()
        {
            // Place your imgui code here.
        }
    }

Now in `userinterface.cpp` (or any equivalent file if you're not working on the leveleditor) you need to call `InterfaceManager::RegisterInterface` to register the interface to the rendering queue.
    
    Interface::InterfaceManager::Instance()->RegisterInterface("Interface::<SUBCLASSNAME>", "Interface Label");

That's it! 