#pragma once

#include "component.h"
#include "message_callback.h"
#include "system.h"

class MessageComponent : public Component<MessageComponent>, public IAwakeFromPoolSystem<IMessageCallBackFunction*>
{
public:
    // 析构函数
    ~MessageComponent();
    
    // 从对象池唤醒时调用
    void AwakeFromPool(IMessageCallBackFunction* pCallback) override;
    
    // 返回对象池时调用
    void BackToPool() override;

    // 判断是否关注消息ID
    bool IsFollowMsgId(Packet* packet) const;
    
    // 处理包
    void ProcessPacket(Packet* packet) const;

protected:
    IMessageCallBackFunction* _pCallBackFuns{ nullptr }; // 回调函数指针
};
