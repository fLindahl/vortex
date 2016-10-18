#pragma once
#include "config.h"
#include "foundation/messaging/messagehandler.h"
#include "foundation/math/vector4.h"
#include "render/properties/graphicsproperty.h"

struct Msg;

namespace Game
{

class Entity
{
public:
	Entity();
	~Entity();

	int getID() { return ID; };
	void setID(const int id) { ID = id; };

	virtual void FixedUpdate();
	virtual void Update();

	virtual void HandleMsg(Msg msg);
	virtual void SendMsg(Msg msg);
	virtual void SendMsg(int recipientID, MsgType message, float delay);

	virtual Math::Vector4& getPos();
	virtual void setPos(Math::Vector4 nPos);

	std::shared_ptr<Render::GraphicsProperty> getGraphicsProperty() { return graphics; }

protected:
	int ID;
	Math::Vector4 pos;

	std::shared_ptr<Render::GraphicsProperty> graphics;
};

}