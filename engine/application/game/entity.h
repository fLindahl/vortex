#pragma once
#include "config.h"
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
#include "application/basegamefeature/managers/entitymanager.h"
#include "foundation/messaging/messagehandler.h"
#include "foundation/math/vector4.h"
#include "entitymessages.h"
#include "foundation/math/bbox.h"

namespace Game
{
	class BaseProperty;
}

#define __SendMsg(OBJ, MSG) OBJ->HandleMessage(MSG.downcast<Messaging::Message>())

namespace Game
{
class Entity : public Core::RefCounted
{

	__DeclareClass(Entity);
public:
	Entity();
	~Entity();

	const uint& GetID() const { return ID; };
	//void SetID(const int id) { ID = id; };

	///Activates this entity and all it's properties
	virtual void Activate();
	///Deactivates this entity and all it's properties
	virtual void Deactivate();

	///Updates this entity and calls all its properties FixedUpdate() functions. This is called much less frequently than Update()
	virtual void FixedUpdate();
	///Updates this entity and calls all its properties Update() functions.
	virtual void Update();

	///Handles a message and dispatches it its properties
	virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
	
	///Append a property to this entity
    void AddProperty(const Ptr<Game::BaseProperty>& p);
	///Remove a property from this entity
	///Note: This calls the propertys destructor!
	void RemoveProperty(const Ptr<Game::BaseProperty>& p);

	///Returns the property located at index
	Ptr<Game::BaseProperty>& Property(uint index);

	///Return the given property template if it exists. Remember to check if ptr is valid before using retrieved property!
	///This should be used sparingly since it's slow.
	template<class PROPERTY> const Ptr<PROPERTY>& FindProperty() const;

	///Shortcut for getting this entitys transform
	virtual Math::mat4 GetTransform();
	///Shortcut for setting this entitys transform
	virtual void SetTransform(const Math::mat4& nTransform);

	///Returns the number of properties that this entity contains
	size_t GetNumProperties();

	//virtual void Serialize();

	Math::bbox& GetBBox() { return this->bbox; }
	const Math::bbox& GetBaseBBox() { return *this->baseBBox; }
	void SetBaseBBox(const Math::bbox* box);

	const bool& IsActive() const;

protected:
	///Set to true when Activate is called. Set to false when Deactivat is called.
	bool active = false;

	///This entitys transform
    Math::mat4 transform;
	///This entitys axis-aligned bounding box
	Math::bbox bbox;
	///Constant pointer to the bounding box in identity space. This is a direct pointer to the mesh base bbox.
	const Math::bbox* baseBBox;

	///Unique ID for this entity.
	uint ID;

	///List of all this netitys properties.
	Util::Array<Ptr<Game::BaseProperty>> properties;

private:
	///Updates the bounding box bounds based on this entitys transform
	void UpdateBBox();
};

template<class PROPERTY>
const Ptr<PROPERTY>& Entity::FindProperty() const
{	
	Core::Rtti* rtti = &PROPERTY::RTTI;
	for (int i = 0; i < this->properties.Size(); i++)
	{
		Core::Rtti* pRtti = this->properties[i]->GetRtti();
		if (pRtti == rtti)
		{
			return this->properties[i].downcast<PROPERTY>();
		}
	}

	return nullptr;
}

}