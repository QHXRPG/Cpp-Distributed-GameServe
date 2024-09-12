#include "message_component.h"

// 析构函数
MessageComponent::~MessageComponent()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;
        _pCallBackFuns = nullptr;
    }
}

// 从对象池唤醒时调用
void MessageComponent::AwakeFromPool(IMessageCallBackFunction* pCallback)
{
    _pCallBackFuns = pCallback;
}

// 返回对象池时调用
void MessageComponent::BackToPool()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;
        _pCallBackFuns = nullptr;
    }
}

// 判断是否关注消息ID
bool MessageComponent::IsFollowMsgId(Packet* packet) const
{
    if (_pCallBackFuns == nullptr)
        return false;

    return _pCallBackFuns->IsFollowMsgId(packet);
}

// 处理包
void MessageComponent::ProcessPacket(Packet* pPacket) const
{
    if (_pCallBackFuns == nullptr)
        return;

    _pCallBackFuns->ProcessPacket(pPacket);
}
