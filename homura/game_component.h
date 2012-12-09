// Ŭnicode please
#pragma once

#include "message_handler.h"

#if SR_USE_PCH == 0
#include "game_globals.h"
#endif

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

// 예제코드를 위해서만 존재하고 실제로는 사용하지 않는다.
// null포인터로 대부분의 역할이 넘어가지니까 굳이 쓸 이유는 없다
class NullComponent : public GameComponent {
public:
    explicit NullComponent(GameObject *obj) : GameComponent(obj) {}
    ~NullComponent() {}

    CompType type() const { return kCompNull; }
    void Update(float dt) {}
    void InitMsgHandler() {}
};