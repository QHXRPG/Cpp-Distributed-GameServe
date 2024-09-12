#include "message_component.h"

// ��������
MessageComponent::~MessageComponent()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;
        _pCallBackFuns = nullptr;
    }
}

// �Ӷ���ػ���ʱ����
void MessageComponent::AwakeFromPool(IMessageCallBackFunction* pCallback)
{
    _pCallBackFuns = pCallback;
}

// ���ض����ʱ����
void MessageComponent::BackToPool()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;
        _pCallBackFuns = nullptr;
    }
}

// �ж��Ƿ��ע��ϢID
bool MessageComponent::IsFollowMsgId(Packet* packet) const
{
    if (_pCallBackFuns == nullptr)
        return false;

    return _pCallBackFuns->IsFollowMsgId(packet);
}

// �����
void MessageComponent::ProcessPacket(Packet* pPacket) const
{
    if (_pCallBackFuns == nullptr)
        return;

    _pCallBackFuns->ProcessPacket(pPacket);
}
