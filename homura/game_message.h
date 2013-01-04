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

#define GAME_MESSAGE_BEGIN(name) struct name : public GameMessage {\
private:\
    name () {}\
public:\
    GameMessage *Clone() const { return new name (); }

#define GAME_MESSAGE_END };

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

//객체 초기화를 요청하는 메시지
struct ResetMessage : public GameMessage {
private:
    ResetMessage() {}
public:
    static ResetMessage Create();
    GameMessage *Clone() const { return new ResetMessage(); }
};

//Physics Component Messages

//게임 객체를 특정 위치로 이동시켜달라고 요청할 때 사용하는 메시지
//msg에 들어온 값만큼 이동시켜줌.
struct MoveToMessage : public GameMessage {
private:
    MoveToMessage() {}
public:
    static MoveToMessage Create(const glm::vec2 &vec, float duration);
    GameMessage *Clone() const { return new MoveToMessage(); }

    glm::vec2 vec;
    float duration;
};

//게임 객체를 delta값만큼 이동시켜달라고 요청
struct MoveByMessage : public GameMessage {
private:
    MoveByMessage() {}
public:
    static MoveByMessage Create(const glm::vec2 &vec, float duration);
    GameMessage *Clone() const { return new MoveByMessage(); }

    glm::vec2 vec;
    float duration;
};


//이거 쓰지 말고 아래거 쓰기
//지울 예정
struct SetAngleMessage : public GameMessage {
private:
    SetAngleMessage() {}
public:
    static SetAngleMessage Create(float angle);
    GameMessage *Clone() const { return new SetAngleMessage(); }

    float angle;
};

struct SetPhyBodyInfoMessage : public GameMessage {
private:
    SetPhyBodyInfoMessage() {}
public:
    static SetPhyBodyInfoMessage Create(PhyBodyInfo *info);
    GameMessage *Clone() const { return new SetPhyBodyInfoMessage(); }

    PhyBodyInfo *info;
};

struct RequestPhyBodyInfoMessage : public GameMessage {
private:
    RequestPhyBodyInfoMessage() {}
public:
    static RequestPhyBodyInfoMessage Create(PhyBodyInfo *info);
    GameMessage *Clone() const { return new RequestPhyBodyInfoMessage(); }

    PhyBodyInfo *phy_body_info;
    //body_info가 제대로 들어갔는지 확인하기 위한 변수
    bool is_ret;
};

//End Physics Component Messages

//AI Component Messages

struct IsEnemyMessage : public GameMessage {
private:
    IsEnemyMessage() {}
public:
    static IsEnemyMessage Create();
    GameMessage *Clone() const { return new IsEnemyMessage(); }
    
    bool is_enemy;
    bool is_ret;
};

//End AI Component Messages

//Player Component Messages

struct CollidePlaneMessage : public GameMessage {
private:
    CollidePlaneMessage() {}
public:
    static CollidePlaneMessage Create(GameObject *counter_obj);
    GameMessage *Clone() const { return new CollidePlaneMessage(); }

    GameObject *counter_obj;
};

//End Player Component Messages

//Character Component Messages
//캐릭터 자체에 데미지를 주는 메시지
struct DamageObjectMessage : public GameMessage {
private:
    DamageObjectMessage() {}
public:
    static DamageObjectMessage Create(float damage);
    GameMessage *Clone() const { return new DamageObjectMessage(); }

    float damage;
};

//이거랑 플레이어 가져다 쓰는거랑 성능차는?
struct RequestPlayerPositionMessage : public GameMessage {
private:
    RequestPlayerPositionMessage() {}
public:
    static RequestPlayerPositionMessage Create(b2Vec2 *position);
    GameMessage *Clone() const { return new RequestPlayerPositionMessage(); }

    b2Vec2 *position;
    bool is_ret;
};

struct RequestRecoveryMessage : public GameMessage {
private:
    RequestRecoveryMessage() {}
public:
    static RequestRecoveryMessage Create(CharacterComponent *char_comp);
    GameMessage *Clone() const { return new RequestRecoveryMessage(); }

    CharacterComponent *char_comp;
};

//쉴드를 생성하라는 메시지
struct CreateShieldMessage : public GameMessage {
private:
    CreateShieldMessage() {}
public:
    static CreateShieldMessage Create(bool from_enemy);
    GameMessage *Clone() const { return new CreateShieldMessage(); }

    //적도 필살기를 쓸 수 있지 않을까하는 가능성을 위해.
    bool from_enemy;
};

//Character Component Messages

struct AttackMessage : public GameMessage {
private:
    AttackMessage() {}
public:
    static AttackMessage Create(int target_id);
    GameMessage *Clone() const { return new AttackMessage(); }

    int target_id;
};

struct CollideBulletMessage : public GameMessage {
private:
    CollideBulletMessage() {}
public:
    static CollideBulletMessage Create(GameObject *bullet, float damage, bool from_enemy);
    GameMessage *Clone() const { return new CollideBulletMessage(); }

    GameObject *bullet;
    float damage;
    bool from_enemy;
    bool applied;
};

//End Character Component Messages

//Bullet Component Messages

//총알과 게임 객체와의 충돌했을 때, 총알에게 넘어가는 메시지
struct BulletDamageObjectMessage : public GameMessage {
private:
    BulletDamageObjectMessage() {}
public:
    static BulletDamageObjectMessage Create(GameObject *obj);
    GameMessage *Clone() const { return new BulletDamageObjectMessage(); }

    GameObject *obj;
};


//End Bullet Component Messages

//Bound Check Component Messages

struct OutOfBoundMessage : public GameMessage {
private:
    OutOfBoundMessage() {}
public:
    static OutOfBoundMessage Create(const b2Vec2 &prev_pos, const b2Vec2 &current_pos);
    GameMessage *Clone() const { return new OutOfBoundMessage(); }

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
    GameMessage *Clone() const { return new BeginTriggerMessage(); }

    int trigger_id;
};

//End Game Event Component Messages

//아군 수 체크 메시지
struct CheckForcesNumberMessage : public GameMessage {
private:
    CheckForcesNumberMessage() {}
public:
    static CheckForcesNumberMessage Create(bool is_enemy);
    GameMessage *Clone() const { return new CheckForcesNumberMessage(); }

    //아군 수를 셀 때는 false, 적군 수를 셀 때는 true로 둔다
    bool is_enemy;
    int forces_number;
};

//편대 관련 메시지

//편대에 가입 요청
struct RequestJoinFormationMessage : public GameMessage {
private:
    RequestJoinFormationMessage() {}
public:
    static RequestJoinFormationMessage Create(int id);
    GameMessage *Clone() const { return new RequestJoinFormationMessage(); }

    int id;
};

//체인 관련 메시지

//캐릭터 객체와 연결된 체인이 있는지 확인
struct CheckConnectedChainMessage : public GameMessage {
private:
    CheckConnectedChainMessage() {}
public:
    static CheckConnectedChainMessage Create(int id);
    GameMessage *Clone() const { return new CheckConnectedChainMessage(); }

    int id;
    bool checked;
};


//월드를 통해서 직접 접근해도 괜찮지 않나?
//이건 놔두고 쉴드 레이어는 직접 접근해서 그려보자.
//레이저 레이어 메시지
GAME_MESSAGE_BEGIN(RequestRenderLaserMessage)
    static RequestRenderLaserMessage Create(int id, const glm::vec2 &start_point, const glm::vec2 &end_point);
    
    int id;
    glm::vec2 end_point;
    glm::vec2 start_point;
GAME_MESSAGE_END

GAME_MESSAGE_BEGIN(StopRenderLaserMessage)
    static StopRenderLaserMessage Create(int id);

    int id;
GAME_MESSAGE_END