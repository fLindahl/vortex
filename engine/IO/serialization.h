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
	
}