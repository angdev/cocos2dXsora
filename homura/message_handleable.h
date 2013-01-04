// Ŭnicode please
#ifndef __MESSAGE_HANDLEABLE_H__
#define __MESSAGE_HANDLEABLE_H__

#include "message_handler.h"

class MessageHandleable {
public:
    MessageHandleable() {}
    virtual ~MessageHandleable() {}

public:
    //message
    void OnMessage(const GameMessage *msg) {
        msg_handler_.HandleMsg(msg);
    }

protected:
    template<typename T, typename MsgT>
    void RegisterMsgFunc(T *instance, void (T::*mem_fn)(MsgT*)) {
        msg_handler_.RegisterMessageFunc(instance, mem_fn);
    }
    
private:
    MessageHandler msg_handler_;
};

#endif