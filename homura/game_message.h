// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "sora/shared_ptr_include.h"
#include "glm/glm.hpp"
#endif

struct GameMessage;
struct DelayedGameMessage;
struct PhyBodyInfo;

class CharacterComponent;

typedef std::shared_ptr<DelayedGameMessage> DelayedGameMessagePtr;
typedef std::shared_ptr<GameMessage> GameMessagePtr;

struct GameMessage {
    GameMessage() : valid(true) { }
	virtual ~GameMessage() {}
	virtual GameMessage *Clone() const = 0;

	//메세지가 유효한가? 메세지를 무효화라는 기능을 추가하면
	//다른 객체에서는 메세지를 씹도록 짤수잇으니까
	bool valid;	
};

struct DelayedGameMessage {
	//지연시간을 걸고 메세지를 날리면서
	//메세지를 날리기 위해서는 메세지를 호출한 지점의 msg가 지연된 시간후 호출될때까지
	//남아있을거라는 보장이 없으니 복사가 가능해야한다.
	//근데 상속을 기반으로 메세지를 구성하도록 수정햇으니까
	//동적할당을 적절히 처리하자
	DelayedGameMessage(GameMessage *msg, float delay);

    bool operator<(const DelayedGameMessage &o) const;
    GameMessage *msg_ptr() { return msg.get(); }
    void Update(float dt);

private:
	std::unique_ptr<GameMessage> msg;
    float remain_time_;	//얼마나 시간이 지난뒤에 처리할 것인가
};

// 게임 객체가 파기되는것을 알리기 위한 메시지
struct DestroyMessage : public GameMessage {
private:
	DestroyMessage() {}
public:
	static DestroyMessage Create(int obj_id);
	GameMessage *Clone() const { return new DestroyMessage(); }

	//파괴될 id의 객체
	int obj_id;
};

//Physics Component Messages

//게임 객체를 이동시켜달라고 요청할 때 사용하는 메시지
//msg에 들어온 값만큼 이동시켜줌.
struct MoveMessage : public GameMessage {
private:
    MoveMessage() {}
public:
    static MoveMessage Create(const b2Vec2 &vec);
    MoveMessage *Clone() const { return new MoveMessage(); }

    b2Vec2 vec;
};

struct SetAngleMessage : public GameMessage {
private:
    SetAngleMessage() {}
public:
    static SetAngleMessage Create(float angle);
    SetAngleMessage *Clone() const { return new SetAngleMessage(); }

    float angle;
};

struct RequestPhyBodyInfoMessage : public GameMessage {
private:
    RequestPhyBodyInfoMessage() {}
public:
    static RequestPhyBodyInfoMessage Create(PhyBodyInfo *info);
    RequestPhyBodyInfoMessage *Clone() const { return new RequestPhyBodyInfoMessage(); }

    PhyBodyInfo *phy_body_info;
    //body_info가 제대로 들어갔는지 확인하기 위한 변수
    bool is_ret;
};

//End Physics Component Messages

//Player Component Messages


//End Player Component Messages

//이거랑 플레이어 가져다 쓰는거랑 성능차는?
struct RequestPlayerPositionMessage : public GameMessage {
private:
    RequestPlayerPositionMessage() {}
public:
    static RequestPlayerPositionMessage Create(b2Vec2 *position);
    RequestPlayerPositionMessage *Clone() const { return new RequestPlayerPositionMessage(); }

    b2Vec2 *position;
    bool is_ret;
};

struct RequestRecoveryMessage : public GameMessage {
private:
    RequestRecoveryMessage() {}
public:
    static RequestRecoveryMessage Create(CharacterComponent *char_comp);
    RequestRecoveryMessage *Clone() const { return new RequestRecoveryMessage(); }

    CharacterComponent *char_comp;
};
//Character Component Messages


struct CollideBulletMessage : public GameMessage {
private:
    CollideBulletMessage() {}
public:
    static CollideBulletMessage Create(GameObject *bullet, float damage, bool from_enemy);
    CollideBulletMessage *Clone() const { return new CollideBulletMessage(); }

    GameObject *bullet;
    float damage;
    bool from_enemy;
    bool applied;
};

//End Character Component Messages

//Bullet Component Messages

struct DamageObjectMessage : public GameMessage {
private:
    DamageObjectMessage() {}
public:
    static DamageObjectMessage Create(GameObject *obj);
    DamageObjectMessage *Clone() const { return new DamageObjectMessage(); }

    GameObject *obj;
};


//End Bullet Component Messages

//Bound Check Component Messages

struct OutOfBoundMessage : public GameMessage {
private:
    OutOfBoundMessage() {}
public:
    static OutOfBoundMessage Create(const b2Vec2 &prev_pos, const b2Vec2 &current_pos);
    OutOfBoundMessage *Clone() const { return new OutOfBoundMessage(); }

    b2Vec2 prev_pos;
    b2Vec2 current_pos;
};

//End Bound Check Component Messages

//Game Event Component Messages

struct BeginTriggerMessage : public GameMessage {
private:
    BeginTriggerMessage() {}
public:
    static BeginTriggerMessage Create(int trigger_id);
    BeginTriggerMessage *Clone() const { return new BeginTriggerMessage(); }

    int trigger_id;
};

//End Game Event Component Messages