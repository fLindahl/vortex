#pragma once
/**
	@file	serialization.h

	Provides helper functions and macros for serializing/deserializing objects.

	@copyright	See LICENSE file
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "tinyxml2.h"


//------------------------------------------
/**
	@macro	__StartSerializing()

	This opens a new xml document for writing.
	You will need to close the document with __EndSerializing(Util::String fileString)

*/
#define __StartSerializing() \
tinyxml2::XMLDocument data; \
tinyxml2::XMLNode* pRoot = data.NewElement("Vortex"); \
data.InsertFirstChild(pRoot);

//------------------------------------------
/**
	@macro	__StopSerializing(Util::String fileString)

	This save a serialization xml file to fileString path

*/
#define __StopSerializing(fileString) \
tinyxml2::XMLError eResult = data.SaveFile(fileString.AsCharPtr()); \
_assert(!eResult);

//------------------------------------------
/**
	@macro	__SerializeValue()

	Writes a value to the open .xml file

	Make sure that __StartSerializing has been called in within the same function.
*/

namespace IO
{
	static void test()
	{
		tinyxml2::XMLDocument data;

		tinyxml2::XMLNode* pRoot = data.NewElement("Vortex");
		data.InsertFirstChild(pRoot);

		tinyxml2::XMLNode* docks = data.NewElement("Docks");
		pRoot->InsertFirstChild(docks);

		for (int i = 0; i < m_docks.size(); ++i)
		{
			Dock& dock = *m_docks[i];
			tinyxml2::XMLElement* element = data.NewElement("Dock");
			element->SetAttribute("index", i);
			element->SetAttribute("label", dock.label);
			element->SetAttribute("x", (int)dock.pos.x);
			element->SetAttribute("y", (int)dock.pos.y);
			element->SetAttribute("location", dock.location);
			element->SetAttribute("size_x", (int)dock.size.x);
			element->SetAttribute("size_y", (int)dock.size.y);
			element->SetAttribute("status", (int)dock.status);
			element->SetAttribute("active", (int)dock.active);
			element->SetAttribute("opened", (int)dock.opened);
			element->SetAttribute("prev", (int)getDockIndex(dock.prev_tab));
			element->SetAttribute("next", (int)getDockIndex(dock.next_tab));
			element->SetAttribute("child0", (int)getDockIndex(dock.children[0]));
			element->SetAttribute("child1", (int)getDockIndex(dock.children[1]));
			element->SetAttribute("parent", (int)getDockIndex(dock.parent));

			docks->InsertEndChild(element);
		}

		tinyxml2::XMLError eResult = data.SaveFile(filename.c_str());
		assert(!eResult);
	}

}