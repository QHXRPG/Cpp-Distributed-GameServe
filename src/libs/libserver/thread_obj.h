#pragma once

#include "disposable.h"
#include "message_list.h"

// ÿ���̰߳����඼��һ��Actor
class ThreadObject : public IDisposable, public MessageList
{
public:
    virtual ~ThreadObject () {}

    // ��ʼ������
    virtual bool Init() = 0;

    // ע�����Ȥ��Э��
    virtual void RegisterMsgFunction() = 0;

    // ֡����
    virtual void Update() = 0;

    bool IsActive() const;

    virtual void Dispose() override;

protected:
    bool _active {true};
};