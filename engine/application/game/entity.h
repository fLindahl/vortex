
/*
#pragma once
#include "config.h"
#include "messages/MsgHandler.h"
#include "foundation/math/vector2.h"
#include "render/properties/graphicsproperty.h"

struct Msg;

namespace Game
{

class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	const int getID() { return ID; };
	void setID(const int id) { ID = id; };

	virtual void FixedUpdate();
	virtual void Update();

	virtual void HandleMsg(Msg msg);
	virtual void SendMsg(Msg msg);
	virtual void SendMsg(int recipientID, MsgType message, float delay);

	virtual Math::Vector2& getPos();
	virtual void setPos(Math::Vector2 nPos);

	std::shared_ptr<Render::GraphicsProperty> getGraphicsProperty() { return graphics; }

protected:
	int ID;
	Math::Vector2 pos;

	std::shared_ptr<Render::GraphicsProperty> graphics;

};

}

 */