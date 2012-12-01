// Ŭnicode please
#pragma once

#include "message_handler.h"

class GameObject;
struct GameMessage;

class GameComponent {
public:
    explicit GameComponent(GameObject *obj);
	virtual ~GameComponent();

    virtual CompType type() const = 0;
	virtual void Update(float dt) = 0;
	virtual void InitMsgHandler() = 0;

	const GameObject *obj() const { return obj_; }
	GameObject *obj() { return obj_; }

	void OnMessage(const GameMessage *msg);

protected:
	template<typename T, typename MsgT>
	void RegisterMsgFunc(T *instance, void (T::*mem_fn)(MsgT*)) {
		msg_handler_.RegisterMessageFunc(instance, mem_fn);
	}

private:
	GameObject *obj_;
	MessageHandler msg_handler_;
};
