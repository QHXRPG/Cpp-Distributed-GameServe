#pragma once

#include "disposable.h"

// ÿ���̰߳����඼��һ��Actor
class ThreadObject : public IDisposable
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

protected:
    bool _active {true};
};