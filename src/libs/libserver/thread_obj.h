#pragma once

#include "disposable.h"
#include "message_list.h"

// 每个线程包裹类都是一个Actor
class ThreadObject : public IDisposable, public MessageList
{
public:
    virtual ~ThreadObject () {}

    // 初始化函数
    virtual bool Init() = 0;

    // 注册感兴趣的协议
    virtual void RegisterMsgFunction() = 0;

    // 帧函数
    virtual void Update() = 0;

    bool IsActive() const;

    virtual void Dispose() override;

protected:
    bool _active {true};
};