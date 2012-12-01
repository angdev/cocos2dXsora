// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "sora/shared_ptr_include.h"
#endif

struct GameMessage;
struct DelayedGameMessage;

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
