#pragma once
#include "application/game/baseproperty.h"


	
namespace Property
{

class FreeCamera : public Game::BaseProperty
{
public:
	FreeCamera();
	~FreeCamera();

private:

	/*
	Creates a serializable variable.
	__Attribute(type, varName, 'FOURCC');
	
	#define __Serializable(type, name) \
		type name; \
		const char* name##Str = #name; \
		type Get##name() { return this->name; } \
		type Set##name(type val) { this->name = val; } \

	Remember to do __RegisterAttribute(varName) in the constructor too.
	This adds the varible to the attribute list.

	Game::Attribute is a class kinda like Util::Variable, but it contains a void pointer to the original value within the class that owns it.
	It also contains a type identifier and getfunctions for the different types that just casts the void pointer to the type we need.
	
	Filling this list will enable us to loop through the list in our serializer/deserializer and easily get the values.
	Util::Array<Game::Attribute> attributeList;
	
	*/
};

}