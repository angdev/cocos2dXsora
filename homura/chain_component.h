// Ŭnicode please
#ifndef __CHAIN_COMPONENT_H__
#define __CHAIN_COMPONENT_H__

#include "logic_component.h"

class GameObject;


//편대는 게임 월드에 하나 존재해서 기체들을 이리저리 조정하지만
//체인은 여러 개 존재할 수 있어서 적 기체의 움직임에 따라서 붙잡힌 아군 기체를
//적절히 조정
class ChainComponent : public LogicComponent {
public:
    explicit ChainComponent(GameObject *obj);
    virtual ~ChainComponent();

public:
    virtual CompType type() const;
    virtual void Update(float dt);
    virtual void InitMsgHandler();

    void Destroy();

public:
    void OnDestroyMessage(DestroyMessage *msg);
    void OnCheckConnectedChainMessage(CheckConnectedChainMessage *msg);
    void OnRemoveChainPartnerMessage(RemoveChainPartnerMessage *msg);

public:
    void set_master_id(int master_id) { master_id_ = master_id; }
    void set_slave_id(int slave_id) { slave_id_ = slave_id; }

private:
    int master_id_;
    int slave_id_;

};

#endif