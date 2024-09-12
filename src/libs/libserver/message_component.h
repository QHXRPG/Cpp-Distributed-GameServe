#pragma once

#include "component.h"
#include "message_callback.h"
#include "system.h"

class MessageComponent : public Component<MessageComponent>, public IAwakeFromPoolSystem<IMessageCallBackFunction*>
{
public:
    // ��������
    ~MessageComponent();
    
    // �Ӷ���ػ���ʱ����
    void AwakeFromPool(IMessageCallBackFunction* pCallback) override;
    
    // ���ض����ʱ����
    void BackToPool() override;

    // �ж��Ƿ��ע��ϢID
    bool IsFollowMsgId(Packet* packet) const;
    
    // �����
    void ProcessPacket(Packet* packet) const;

protected:
    IMessageCallBackFunction* _pCallBackFuns{ nullptr }; // �ص�����ָ��
};
