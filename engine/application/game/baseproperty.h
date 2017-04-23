#pragma once
#include "core/refcounted.h"
#include "foundation/messaging/messagehandler.h"
#include "entity.h"
#include "application/game/attribute.h"
#include "foundation/util/fourcc.h"
#include "foundation/util/dictionary.h"


//---------------------------------------
/**
@macro	__Attribute(name)

Defines an attribute of a property


*/
#define __Attribute(type, name) \
	type name; \
	Util::FourCC name##_FourCC; \
	void Set##name##CallBack(const type& value);

//---------------------------------------
/**
@macro	__SerializeAttribute(name)

Registers the attribute to a property.

@note	Place this iin the constructor of the property

@remark: We might want to create a manager for attributes so that we can easily update attributes without needing to knwo which class owns it.
In that case we need to register the attribute to the RTTI and GUID for this class.
*/
#define __SerializeAttribute(name, fourCC) \
	{\
		Game::Attribute attr(&name, &this->Set##name##CallBack);	\
		this->name##_FourCC = fourCC;		\
		this->attributeList.Add(this->name##_FourCC, attr);	\
	}
	
//---------------------------------------
/**
	@class	Game::BaseProperty

	
*/	
namespace Game
{

class BaseProperty : public Core::RefCounted
{
__DeclareClass(BaseProperty)
public:
	BaseProperty();
    virtual ~BaseProperty() {}

    virtual void FixedUpdate() {}
    virtual void Update() {}

	virtual void HandleMessage(const Ptr<Messaging::Message>& msg) {}

	virtual void Activate() { active = true; }
	virtual void Deactivate() { active = false; }

	void SetOwner(Ptr<Entity> o) { this->owner = o; }
	const Ptr<Entity>& GetOwner() { return this->owner; }

protected:
	bool active = false;
    friend class Entity;

	///The owner entity of this property
    Ptr<Entity> owner;

	/**
	Contains all the serializable variables for this property
	
	Game::Attribute is a class kinda like Util::Variable, but it contains only a pointer to the original value within the class that owns it and an enum that describes its type.
	It also contains a type identifier and getfunctions for the different types that just casts the void pointer to the type we need.
	Filling this list will enable us to loop through the list in our serializer/deserializer and easily get the values.
	*/
	Util::Dictionary<Util::FourCC, Game::Attribute> attributeList;
};

}